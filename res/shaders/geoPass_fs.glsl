#version 440

in vec3 v_pos;
in vec3 v_nor;
in vec2 v_uvs;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gDiffSpec;

uniform sampler2D placeholder1;
uniform sampler2D placeholder2;

void main() {
  //gPosition = v_pos;
  gPosition = vec3(1.0f, 0.0f, 0.0f);

  gNormal = normalize(v_nor);

  gDiffSpec.rgb = texture(placeholder1, v_uvs).rgb;

  gDiffSpec.a = texture(placeholder2, v_uvs).r;
}
