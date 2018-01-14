#version 440

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_nor;
layout(location = 2) in vec2 in_uvs;
layout(location = 3) in vec3 in_tan;
layout(location = 4) in vec3 in_btan;

out vec3 v_pos;
out vec3 v_nor;
out vec2 v_uvs;
out vec4 v_lgtpos;

// normal mapping
out vec3 v_nor_world; // normal in camera space
out vec3 v_tan_world; // tangent in camera space
out vec3 v_btan_world; // bitangent in camera space
// out vec3 v_worldDir_world; // a vector from vertex to camera in cam space
// out vec3 v_lgtDir_world; // a vector from vertex to light in cam space
// out vec3 v_lgtPos_world; // Position of light in cam space
//
// out vec3 v_worldDir_tan; // a vector in from vertex to camera in tan space
// out vec3 v_lgtDir_tan; // a vector from vertex to light in tan space
// end normal mapping

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat4 light_mat;

void main() {
  vec4 w_pos = model * vec4(in_pos, 1.0);
  vec4 positionRelativeToCamera = view * w_pos;

  gl_Position = perspective * view * w_pos;

  v_pos = vec3( w_pos );   //Save pos in world space

  v_nor = vec3( model * vec4(in_nor, 0.0) );   //Save normals in world space

  v_uvs = in_uvs;

  // NORMALMAPPING

  // Get Normal, Tangent and Bitanget in world space
  v_nor_world  = vec3(model * vec4(in_nor, 0.0f) );
  v_tan_world  = vec3(model * vec4(in_tan, 0.0f) );
  v_btan_world = vec3(model * vec4(in_btan, 0.0f) );
  // END NORMALMAPPING
  //WIP
  v_lgtpos = light_mat * w_pos;
  //v_lgtpos = v_pos; //Temp, just to prevent crashing

}
