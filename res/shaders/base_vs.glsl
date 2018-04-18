#version 440

in vec3 vp;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main() {
  gl_Position = perspective * view * model * vec4(vp, 1.0);
}
