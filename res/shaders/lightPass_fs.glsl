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

uniform PntLight pnt_lights[NR_OF_PNTLIGHTS];
uniform DirLight dir_lights[NR_OF_DIRLIGHTS];
uniform SptLight spt_lights[NR_OF_SPTLIGHTS];

layout (binding=0) uniform sampler2D gPosition;
layout (binding=1) uniform sampler2D gNormal;
layout (binding=2) uniform sampler2D gDiffSpec;
layout (binding=3) uniform sampler2D gLgtPos;
layout (binding=6) uniform sampler2D texture_computed0;
layout (binding=7) uniform sampler2D lDepth;

float shadCalc(vec4 fragPos, vec4 fragNor, vec3 lightPos);
float pcf_shadCalc(vec4 fragPos, vec4 fragNor, vec3 lightPos);

void main() {
	//out_col = texture(gPosition, v_uvs);
  //out_col = texture(gNormal, v_uvs);
  //out_col = texture(gDiffSpec, v_uvs);
  //out_col = vec4(v_uvs, 0.0, 1.0);
/*
  if (v_uvs.y < 0.5) {    //Lower
    if (v_uvs.x < 0.5) {  //Lower Left
      out_col = texture(gPosition, v_uvs);
    }
    else {                //Lower Right
      out_col = texture(gNormal, v_uvs);
    }
  }
  else {                  //Upper
    if (v_uvs.x < 0.5) {  //Upper Left
      out_col = texture(gDiffSpec, v_uvs);
    }
    else {                //Upper Right
      out_col = texture(gLgtPos, v_uvs);
    }
  }
*/
  float shadVal = pcf_shadCalc(texture(gLgtPos, v_uvs), texture(gNormal, v_uvs), pnt_lights[0].pos);
  out_col = texture(gDiffSpec, v_uvs) * (1.0f - shadVal);
  // out_col = texture(texture_computed0, v_uvs) * (1.0f - shadVal);

  out_col += pnt_lights[0].dif * 0.0001 + spt_lights[0].dif * 0.0001; //All uploads must be used or we get a segmentation error
}

float shadCalc(vec4 fragPos, vec4 fragNor, vec3 lightPos) {
	vec3 projPos;
	float lightDepth;
	float fragDepth;
	float bias = 0.005;   // Statc bias
	float shadow = 0.0;		// Instancing at 0.0

	projPos = fragPos.xyz / fragPos.w;				// Go back from the projection space by dividing by w. Range [-w, w] becomes range [-1, 1]

	projPos = (projPos * 0.5) + 0.5;					// Go to range [0, 1]

	lightDepth = texture(lDepth, projPos.xy).r;		// Get the depth that the light saw from shadow-map

	fragDepth = projPos.z;    								// Get the depth "we" see from our current pos

	bias = max(0.05 * dot(vec3(fragNor), lightPos), 0.005);	// Calculate a bias value

	if(lightDepth < (fragDepth - bias)){
		// If  lightDepth is smaller the fragment lies in shadow
		shadow = 1.0;
	}

	return shadow;
}

float pcf_shadCalc(vec4 fragPos, vec4 fragNor, vec3 lightPos) {

	vec3 projPos;
	vec2 texelSize;
	float pcfDepth;
	float shadow = 0.0;
	float bias;         // Dynamic Bias

	projPos = fragPos.xyz / fragPos.w;
	projPos = (projPos * 0.5) + 0.5;

	bias = max(0.05 * dot(vec3(fragNor), lightPos), 0.005);

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
