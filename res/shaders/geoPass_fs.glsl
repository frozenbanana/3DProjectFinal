#version 440

in vec3 g_pos;
in vec3 g_nor;
in vec2 g_uvs;

// normal map
in vec3 g_nor_cam; // normal in camera space
in vec3 g_tan_cam; // tangent in camera space
in vec3 g_btan_cam; // bitangent in camera space
// end normal map

in vec4 g_lgtpos;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gDiffSpec;
layout(location = 3) out vec4 gLgtPos;

layout(binding = 4) uniform sampler2D texture_diffuse0;
layout(binding = 5) uniform sampler2D texture_specular0;
layout(binding = 6) uniform sampler2D texture_normal0;

uniform int n_tex;
void main() {
  //gPosition = g_pos;
  gPosition = g_pos;

  // NORMAL CALCULATIONS
  if(n_tex == 3) {
    mat3 TBN = transpose(mat3(g_tan_cam.x, g_btan_cam.x, g_nor_cam.x,
                              g_tan_cam.y, g_btan_cam.y, g_nor_cam.y,
                              g_tan_cam.z, g_btan_cam.z, g_nor_cam.z));

    gNormal = TBN * (texture(texture_normal0, g_uvs).rgb * 2.0  - 1.0);
  }
  else {
    gNormal = normalize(g_nor);
  }

  gDiffSpec.rgb = texture(texture_diffuse0, g_uvs).rgb;
  //gDiffSpec = vec4(1.0);

  gDiffSpec.a = texture(texture_specular0, g_uvs).r;

  gLgtPos = g_lgtpos;
}
