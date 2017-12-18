#version 440

in vec3 vp;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

out vec3 v_posWorld;

void main() {
  gl_Position = perspective * view * model * vec4(vp, 1.0);
  v_posWorld = vec3(model * vec4(vp, 1.0));
}
