#version 440

#define NR_OF_PNTLIGHTS 1
struct PntLight {
  vec3 pos;
  vec4 amb;
  vec4 dif;
  vec4 spe;
  float con;
  float lin;
  float qua;
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
layout (binding=7) uniform sampler2D texture_computed0;
//layout (rgba8, binding=6) uniform image2D texture_computed0;
layout (binding=8) uniform sampler2D lDepth;

//Functions for lights
vec3 pntLightCalc(PntLight lgt, vec3 frag_pos, vec3 frag_nor, vec3 view_dir, vec4 frag_col);
vec3 dirLightCalc(DirLight lgt, vec3 frag_nor, vec3 view_dir, vec4 frag_col);
vec3 sptLightCalc(SptLight lgt, vec3 frag_pos, vec3 frag_nor, vec4 frag_lpos, vec3 view_dir, vec4 frag_col);

//Functions for shadow mapping
float shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos);
float pcf_shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos);

void main() {
  //GATHER DATA AND CALCULATE SOME STUFF----------------------------------------
  vec3 fragment_w_pos = texture(gPosition, v_uvs).rgb;
  // vec3 fragment_w_nor = texture(gNormal, v_uvs).rgb;
  vec3 fragment_w_nor = texture(gNormal, v_uvs).rgb;
  vec4 fragment_col   = texture(gDiffSpec, v_uvs);
  vec4 fragment_l_pos = texture(gLgtPos, v_uvs);

  vec3 view_dir = normalize(camPos - fragment_w_pos);

  //CALCULATE COLORS------------------------------------------------------------
  vec3 fin_col = vec3(0.0, 0.0, 0.0);

  //Modifiers (May not be 0)
  float pnt_mod = 1.0;
  float dir_mod = 1.0;
  float spt_mod = 1.0;

  for(int i = 0; i < NR_OF_PNTLIGHTS; i++) {
    fin_col += pnt_mod * pntLightCalc(pnt_lights[0], fragment_w_pos, fragment_w_nor, view_dir, fragment_col);
  }

  for(int i = 0; i < NR_OF_DIRLIGHTS; i++) {
    fin_col += dir_mod * dirLightCalc(dir_lights[0], fragment_w_nor, view_dir, fragment_col);
  }

  for(int i = 0; i < NR_OF_SPTLIGHTS; i++) {
    fin_col += spt_mod * sptLightCalc(spt_lights[0], fragment_w_pos, fragment_w_nor, fragment_l_pos, view_dir, fragment_col);
    //SHADOW MAPPING INSIDE
  }

  fin_col = fin_col / (0.8 * (NR_OF_PNTLIGHTS + NR_OF_PNTLIGHTS + NR_OF_SPTLIGHTS));

  out_col = vec4(fin_col, 1.0);

  // if (v_uvs.x < 0.5) {
  //   out_col = texture(texture_computed0, v_uvs);
  //   //ivec2 imgCrds = ivec2( v_uvs.x * 640, v_uvs.y * 480 );
  //   //out_col = imageLoad(texture_computed0, imgCrds);
  // }

  //out_col += pnt_lights[0].dif * 0.01 + dir_lights[0].dif * 0.01 + spt_lights[0].dif * 0.01; //All uploads must be used or we get a segmentation error
}//Main

vec3 pntLightCalc(PntLight lgt, vec3 frag_pos, vec3 frag_nor, vec3 view_dir, vec4 frag_col) {
  vec3 light_dir = normalize(lgt.pos - frag_pos);
  vec3 half_dir = normalize(light_dir + view_dir);

  //vec3 amb_lgt = vec3(light_col * amb_coe);

  float dif_coe = max( dot(frag_nor, light_dir), 0 );

  float spe_coe = pow( max( dot(frag_nor, half_dir), 0.0 ), 1.0 /*M?*/ );

  vec3 amb_col = vec3(lgt.amb) * frag_col.rgb;
  vec3 dif_col = vec3(lgt.dif) * dif_coe * frag_col.rgb;
  vec3 spe_col = vec3(lgt.spe) * spe_coe * frag_col.a;

  float dis = length(lgt.pos - frag_pos);
  float att = 1.0 / (lgt.con + lgt.lin * dis + lgt.qua * dis * dis);

  return (amb_col + dif_col + spe_col) * att;
}

vec3 dirLightCalc(DirLight lgt, vec3 frag_nor, vec3 view_dir, vec4 frag_col) {
  vec3 light_dir = normalize(-lgt.dir);
  vec3 ref_dir = reflect(light_dir, frag_nor);

  float dif_coe = max(dot(frag_nor, light_dir), 0.0);
  float spe_coe = pow( max( dot(view_dir, ref_dir), 0.0 ),  1.0 /*M?*/);

  vec3 amb_col = vec3(lgt.amb) * frag_col.rgb;
  vec3 dif_col = vec3(lgt.dif) * dif_coe * frag_col.rgb;
  vec3 spe_col = vec3(lgt.spe) * spe_coe * frag_col.a;

  return amb_col + dif_col + spe_col;
}

vec3 sptLightCalc(SptLight lgt, vec3 frag_pos, vec3 frag_nor, vec4 frag_lpos, vec3 view_dir, vec4 frag_col) {
  float shad_val = pcf_shadCalc(frag_lpos, frag_nor, lgt.pos);

  vec3 light_dir = normalize(lgt.pos - frag_pos);
  vec3 half_dir = normalize(light_dir + view_dir);

  float dif_coe = max( dot(frag_nor, light_dir), 0 );

  float spe_coe = pow( max( dot(frag_nor, half_dir), 0.0 ), 1.0 /*M?*/ );

  vec3 amb_col = vec3(lgt.amb) * frag_col.rgb;
  vec3 dif_col = vec3(lgt.dif) * dif_coe * frag_col.rgb;
  vec3 spe_col = vec3(lgt.spe) * spe_coe * frag_col.a;


  //return (amb_col + (dif_col + spe_col) * shad_val);  //Soft Shadow
  return ((amb_col + dif_col + spe_col) * shad_val); //Hard Shadow

  //if (v_uvs.x < 0.5) {
  //  return (amb_col + (dif_col + spe_col) * shad_val);  //Soft Shadow
  //}
  //else{
  //  return ((amb_col + dif_col + spe_col) * shad_val); //Hard Shadow
  //}
}

float shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos) {
	vec3 projPos;
	float lightDepth;
	float fragDepth;
	float bias = 0.005;
	float shadow = 0.0;		// Instancing at 0.0

	projPos = fragPos.xyz / fragPos.w;				// Go back from the projection space by dividing by w. Range [-w, w] becomes range [-1, 1]

	projPos = (projPos * 0.5) + 0.5;					// Go to range [0, 1]

	lightDepth = texture(lDepth, projPos.xy).r;		// Get the depth that the light saw from shadow-map

	fragDepth = projPos.z;    								// Get the depth "we" see from our current pos

	//bias = max(0.05 * dot(fragNor, lightPos), 0.005);	//NTS: the normals of the terrain makes the bias remove the shadow

	if(lightDepth < (fragDepth - bias)){
		// If  lightDepth is smaller the fragment lies in shadow
		shadow = 1.0;
	}

	return (1.0 - shadow);
}

float pcf_shadCalc(vec4 fragPos, vec3 fragNor, vec3 lightPos) {

	vec3 projPos;
	vec2 texelSize;
	float pcfDepth;
	float shadow = 0.0;
	float bias = 0.005;

	projPos = fragPos.xyz / fragPos.w;
	projPos = (projPos * 0.5) + 0.5;

	//bias = max(0.05 * dot(fragNor, lightPos), 0.005);  //NTS: the normals of the terrain makes the bias remove the shadow

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

	return (1.0 - shadow);
}
