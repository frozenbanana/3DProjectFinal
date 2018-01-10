#version 440

in vec3 g_pos;
in vec3 g_nor;
in vec2 g_uvs;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gDiffSpec;
// layout(location = 2) out vec4 gDiffSpec;

layout(binding = 3) uniform sampler2D texture_diffuse0;
layout(binding = 4) uniform sampler2D texture_specular0;

uniform int n_tex;

void main() {
  //gPosition = g_pos;
  gPosition = g_pos;

  gNormal = normalize(g_nor);

  gDiffSpec.rgb = texture(texture_diffuse0, g_uvs).rgb;

  gDiffSpec.a = texture(texture_specular0, g_uvs).r;
}
