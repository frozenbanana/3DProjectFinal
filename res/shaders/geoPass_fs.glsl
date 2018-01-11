#version 440

in vec3 g_pos;
in vec3 g_nor;
in vec2 g_uvs;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gDiffSpec;

layout(binding = 3) uniform sampler2D texture_diffuse0;
layout(binding = 4) uniform sampler2D texture_specular0;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
layout(binding = 6) uniform sampler2D texture_normal0;
=======
// layout(binding = 6) uniform sampler2D texture_normal0;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture
=======
// layout(binding = 6) uniform sampler2D texture_normal0;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture
=======
// layout(binding = 6) uniform sampler2D texture_normal0;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture

// smart trick to avoid if statement
// if vec2 = (1, 0) ->  0 + normalize(texture(texture_normal0, g_uvs).rgb) ... has a normal map
// if vec2 = (0, 1) ->  normalize(g_nor) * 1 + 0
// uniform vec2 hasNormalMap;
// uniform int n_tex;

void main() {
  gPosition = g_pos;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
  // gNormal = normalize(texture(texture_normal0, g_uvs).rbg * 2.0 - 1.0) * hasNormalMap.x + normalize(g_nor) * hasNormalMap.y;
=======
  // gNormal = normalize(texture(texture_normal0, g_uvs).rgb * 2.0 - 1.0) * hasNormalMap.x + normalize(g_nor) * hasNormalMap.y;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture
=======
  // gNormal = normalize(texture(texture_normal0, g_uvs).rgb * 2.0 - 1.0) * hasNormalMap.x + normalize(g_nor) * hasNormalMap.y;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture
=======
  // gNormal = normalize(texture(texture_normal0, g_uvs).rgb * 2.0 - 1.0) * hasNormalMap.x + normalize(g_nor) * hasNormalMap.y;
>>>>>>> parent of 88b96a8... something intresting is happening with normal map texture
  // gNormal = normalize(texture(texture_normal0, g_uvs).rgb * 2.0 - 1.0);
  gNormal = normalize(g_nor);

  gDiffSpec.rgb = texture(texture_diffuse0, g_uvs).rgb;

  gDiffSpec.a = texture(texture_specular0, g_uvs).r;
}
