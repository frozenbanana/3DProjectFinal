#version 440

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nor;
layout(location = 2) in vec2 in_uvs;

uniform mat4 model;
uniform mat4 light_mat;


void main() {
  gl_Position = light_mat * model * vec4(in_pos, 1.0);
}
