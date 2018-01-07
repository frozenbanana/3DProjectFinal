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
};

in vec2 v_uvs;

out vec4 out_col;

uniform PntLight pnt_lights[NR_OF_PNTLIGHTS];
uniform DirLight dir_lights[NR_OF_DIRLIGHTS];
uniform SptLight spt_lights[NR_OF_SPTLIGHTS];

layout (binding=0) uniform sampler2D gPosition;
layout (binding=1) uniform sampler2D gNormal;
layout (binding=2) uniform sampler2D gDiffSpec;

void main() {
	//out_col = texture(gPosition, v_uvs);
  //out_col = texture(gNormal, v_uvs);
  out_col = texture(gDiffSpec, v_uvs);
  //out_col = vec4(v_uvs, 0.0, 1.0);

  out_col += pnt_lights[0].dif * 0.01; //All uploads must be used or we get a segmentation error
}