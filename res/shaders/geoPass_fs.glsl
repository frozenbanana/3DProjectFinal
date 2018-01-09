#version 440

in vec3 v_pos;
in vec3 v_nor;
in vec2 v_uvs;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gDiffSpec;

layout(binding = 3) uniform sampler2D texture_diffuse0;
layout(binding = 4) uniform sampler2D texture_specular0;

uniform int n_tex;

void main() {
  //gPosition = v_pos;
  gPosition = v_pos;

  gNormal = normalize(v_nor);

  gDiffSpec.rgb = texture(texture_diffuse0, v_uvs).rgb;

  gDiffSpec.a = texture(texture_specular0, v_uvs).r + 0.01 * n_tex;
}
