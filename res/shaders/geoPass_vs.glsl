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
out vec3 v_nor_cam; // normal in camera space
out vec3 v_tan_cam; // tangent in camera space
out vec3 v_btan_cam; // bitangent in camera space
// out vec3 v_camDir_cam; // a vector from vertex to camera in cam space
// out vec3 v_lgtDir_cam; // a vector from vertex to light in cam space
// out vec3 v_lgtPos_cam; // Position of light in cam space
//
// out vec3 v_camDir_tan; // a vector in from vertex to camera in tan space
// out vec3 v_lgtDir_tan; // a vector from vertex to light in tan space
// end normal mapping

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

  // NORMALMAPPING
  // Get vector from origin to vert pos in cam space
  // vec3 pos_cam = vec3(view * model * vec4(in_pos, 1.0));
  // v_camDir_cam = vec3(0.0f, 0.0f, 0.0f) - pos_cam;
  //
  // // Get vector from vertex to the camera in cam space
  // vec3 lgtPos = vec3(light_mat[3][0], light_mat[3][1], light_mat[3][2]);  // Get light pos from matrix
	// vec3 lghPos_cam = (view * vec4(lgtPos), 1.0f).xyz;
	// v_lgtDir_cam = lgtPos_cam + v_camDir_cam;

  // Get Normal, Tangent and Bitanget in cam space
  v_nor_cam  = vec3(view * model * vec4(in_nor, 1.0f) );
  v_tan_cam  = vec3(view * model * vec4(in_tan, 1.0f) );
  v_btan_cam = vec3(view * model * vec4(in_btan, 1.0f) );

  // Get vector from vertex to light in tan space
  // Get vector from vertex to camera in tan space
  // mat3 TBN = transpose(mat3(v_tan_cam, v_btan_cam, v_nor_cam));
  // v_lgtDir_tan = TBN * v_lgtDir_cam;
  // v_camDir_tan = TBN * v_camDir_cam;
  // END NORMALMAPPING

  //WIP
  v_lgtpos = light_mat * w_pos;
  //v_lgtpos = v_pos; //Temp, just to prevent crashing

}
