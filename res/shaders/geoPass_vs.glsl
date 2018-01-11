#version 440

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nor;
layout(location = 2) in vec2 in_uvs;

out vec3 v_pos;
out vec3 v_nor;
out vec2 v_uvs;
out vec4 v_lgtpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat4 light_mat;

void main() {
  vec4 w_pos = model * vec4(in_pos, 1.0);

  gl_Position = perspective * view * w_pos;

  v_pos = vec3( w_pos );   //Save pos in world space

  v_nor = vec3( model * vec4(in_nor, 0.0) );   //Save normals in world space

  v_uvs = in_uvs;

  //WIP
  v_lgtpos = light_mat * w_pos;
  //v_lgtpos = v_pos; //Temp, just to prevent crashing

}
