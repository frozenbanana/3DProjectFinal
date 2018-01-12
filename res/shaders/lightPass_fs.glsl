#version 440

#define NR_OF_PNTLIGHTS 1
struct PntLight {
  vec3 pos;
  vec4 amb;
  vec4 dif;
  vec4 spe;
};

#define NR_OF_DIRLIGHTS 1
struct DirLight {
  vec3 dir;
  vec4 amb;
  vec4 dif;
  vec4 spe;
};

#define NR_OF_SPTLIGHTS 1
struct SptLight {
  vec3 pos;
  vec4 amb;
  vec4 dif;
  vec4 spe;
  //MATRIX
  mat4 mat;
};

in vec2 v_uvs;

out vec4 out_col;

uniform vec3 camPos;

uniform PntLight pnt_lights[NR_OF_PNTLIGHTS];
uniform DirLight dir_lights[NR_OF_DIRLIGHTS];
uniform SptLight spt_lights[NR_OF_SPTLIGHTS];

layout (binding=0) uniform sampler2D gPosition;
layout (binding=1) uniform sampler2D gNormal;
layout (binding=2) uniform sampler2D gDiffSpec;
layout (binding=3) uniform sampler2D gLgtPos;
layout (binding=6) uniform sampler2D texture_computed0;
layout (binding=7) uniform sampler2D lDepth;

//Functions for light types

//Functions for light colors
vec3 ambCalc(vec4 light_col, float amb_coe);
vec3 difCalc(vec3 light_pos, vec3 frag_pos, vec3 frag_nor, vec4 light_col);
vec3 bp_speCalc(vec3 light_pos, vec3 frag_pos, vec3 view_dir, vec3 frag_nor, vec4 light_col);


//Functions for shadow mapping
float shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos);
float pcf_shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos);

void main() {
  //GATHER DATA AND CALCULATE SOME STUFF----------------------------------------
  vec3 fragment_w_pos = texture(gPosition, v_uvs).rgb;
  vec3 fragment_w_nor = texture(gNormal, v_uvs).rgb;
  vec4 fragment_col   = texture(gDiffSpec, v_uvs);
  vec4 fragment_l_pos = texture(gLgtPos, v_uvs);

  vec3 view_dir = normalize(camPos - fragment_w_pos);

  //CALCULATE COLORS------------------------------------------------------------
  vec3 amb_lgt = ambCalc(pnt_lights[0].dif, 1.0);

  vec3 dif_lgt = difCalc (pnt_lights[0].pos, fragment_w_pos, vec3(fragment_w_nor), pnt_lights[0].dif);

  vec3 spe_lgt = bp_speCalc(pnt_lights[0].pos, fragment_w_pos, view_dir, vec3(fragment_w_nor), pnt_lights[0].spe);

  vec3 fin_lgt = amb_lgt + dif_lgt + spe_lgt;
  fin_lgt = normalize(fin_lgt);

  //SHADOW MAPPING--------------------------------------------------------------

  float shadVal = shadCalc(fragment_l_pos, fragment_w_nor, spt_lights[0].pos);
  shadVal = 1.0f - shadVal; //Invert

  //out_col = vec4( vec3(fragment_col) * fin_lgt * shadVal, 1.0 );
  //out_col = fragment_col * (vec4(amb_lgt, 1.0) + vec4( dif_lgt, 1.0 ) + vec4(spe_lgt, 1.0));
  out_col = fragment_col * vec4(fin_lgt, 1.0) * shadVal;

  out_col += pnt_lights[0].dif * 0.01 + dir_lights[0].dif * 0.01 + spt_lights[0].dif * 0.01; //All uploads must be used or we get a segmentation error

}

vec3 ambCalc(vec4 light_col, float amb_coe){
	return  vec3(light_col * amb_coe);
}

vec3 difCalc(vec3 light_pos, vec3 frag_pos, vec3 frag_nor, vec4 light_col) {
  vec3 light_dir = normalize(light_pos - frag_pos);
  float coe = max( dot(frag_nor, light_dir), 0 );
  return light_col * coe;
}

//vec4 speCalc(vec3 worldPos, vec3 normal, vec3 lightDir, float coe){
//	float fragSpe = texture(gDiffSpec, vTex).a;
//
//	vec3 lightSpe = vec3(0.8f);		//MAKE UNIFORM
//
//	vec3 viewDir = normalize(camPos - worldPos);
//
//	vec3 refDir = normalize(reflect(-lightDir, normal));
//
//	float coe_B = max(dot(viewDir, refDir), 0.0f);
//
//	if(coe_B >= 0.0f){
//		coe_B = pow(coe_B, 32);
//	}
//
//	vec3 lightSpe_B = coe * coe_B * lightSpe;
//
//	return vec4(lightSpe_B * fragSpe, 0.0f);
//
//}

vec3 bp_speCalc(vec3 light_pos, vec3 frag_pos, vec3 view_dir, vec3 frag_nor, vec4 light_col) {
  vec3 light_dir = normalize(light_pos - frag_pos);
  vec3 half_dir = normalize(light_dir + view_dir);
  float coe = pow( max( dot(frag_nor, half_dir), 0.0 ), 0.5 );
  return light_col * coe;
}

float shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos) {
	vec3 projPos;
	float lightDepth;
	float fragDepth;
	float bias = 0.005;   // Statc bias
	float shadow = 0.0;		// Instancing at 0.0

	projPos = fragPos.xyz / fragPos.w;				// Go back from the projection space by dividing by w. Range [-w, w] becomes range [-1, 1]

	projPos = (projPos * 0.5) + 0.5;					// Go to range [0, 1]

	lightDepth = texture(lDepth, projPos.xy).r;		// Get the depth that the light saw from shadow-map

	fragDepth = projPos.z;    								// Get the depth "we" see from our current pos

	bias = max(0.05 * dot(fragNor, lightPos), 0.005);	// Calculate a bias value

	if(lightDepth < (fragDepth - bias)){
		// If  lightDepth is smaller the fragment lies in shadow
		shadow = 1.0;
	}

	return shadow;
}

float pcf_shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos) {

	vec3 projPos;
	vec2 texelSize;
	float pcfDepth;
	float shadow = 0.0;
	float bias;         // Dynamic Bias

	projPos = fragPos.xyz / fragPos.w;
	projPos = (projPos * 0.5) + 0.5;

	bias = max(0.05 * dot(fragNor, lightPos), 0.005);

	texelSize = 1.0/textureSize(lDepth, 0);			//One texel equals the size of 1 divided by the size of the texture map
														//the 0 here means that the map is at mipmap level 0

	for(int u = -1; u <= 1; ++u){
		for(int v = -1; v <= 1; ++v){
			pcfDepth = texture(lDepth, projPos.xy + vec2(u, v) * texelSize).r;	//Get the depth value from the map around the main pixel
																					//This goes over	[-1, 1][0, 1][1, 1]
																					//					      [-1, 0][0, 0][1, 0]
																					//					      [-1,-1][0,-1][1,-1]		9x9
																					//Increase start and stop values to get smoother shadows

			shadow = shadow + ((pcfDepth < (projPos.z - bias)) ? 1.0f : 0.0f);		//Calculate shadow additivly
		}
	}

	shadow = shadow/9.0f;	//Divide the shadow by number of texels sampled from

	return shadow;
}
