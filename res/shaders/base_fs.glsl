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

};

uniform PntLight pnt_lights[NR_OF_PNTLIGHTS];
uniform DirLight dir_lights[NR_OF_DIRLIGHTS];
uniform SptLight spt_lights[NR_OF_SPTLIGHTS];

out vec4 FragColor;

void main() {
  FragColor = pnt_lights[0].dif;
}
