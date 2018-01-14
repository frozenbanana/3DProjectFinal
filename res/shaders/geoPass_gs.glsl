#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 camPos;
uniform mat4 model;
// in vec3 v_posWorld[];
in vec3 v_camPos[];
in vec3 v_pos[];
in vec3 v_nor[];
in vec2 v_uvs[];
in vec4 v_lgtpos[];

// normal mapping
in vec3 v_nor_cam[]; // normal in camera space
in vec3 v_tan_cam[]; // tangent in camera space
in vec3 v_btan_cam[]; // bitangent in camera space
// in vec3 v_camDir_cam[]; // a vector from vertex to camera in cam space
// in vec3 v_lgtDir_cam[]; // a vector from vertex to light in cam space
// in vec3 v_lgtPos_cam[]; // Position of light in cam space
// in vec3 v_camDir_tan[]; // a vector in from vertex to camera in tan space
// in vec3 v_lgtDir_tan[]; // a vector from vertex to light in tan space


out vec3 g_nor_cam; // normal in camera space
out vec3 g_tan_cam; // tangent in camera space
out vec3 g_btan_cam; // bitangent in camera space
// out vec3 g_camDir_cam; // a vector from vertex to camera in cam space
// out vec3 g_lgtDir_cam; // a vector from vertex to light in cam space
// out vec3 g_lgtPos_cam; // Position of light in cam space
// out vec3 g_camDir_tan; // a vector in from vertex to camera in tan space
// out vec3 g_lgtDir_tan; // a vector from vertex to light in tan space
// end normalmappoutg

out vec3 g_pos;
out vec3 g_nor;
out vec2 g_uvs;

out vec4 g_lgtpos;

void processPrimitive();
bool cullPrimitive();

void main(){
  if(cullPrimitive()){
    processPrimitive();
  }
}

void processPrimitive(){

  for(int i = 0; i < gl_in.length(); i++){
    gl_Position = gl_in[i].gl_Position;
    g_pos = v_pos[i];
    g_nor = v_nor[i];
    g_uvs = v_uvs[i];
    g_nor_cam = v_nor_cam[i];  // normal in camera space
    g_tan_cam = v_tan_cam[i];  // tangent in camera space
    g_btan_cam = v_btan_cam[i]; // bitangent in camera space
    // g_camDir_cam = v_camDir_cam[i]; // a vector from vertex to camera in cam space
    // g_lgtDir_cam = v_lgtDir_cam[i]; // a vector from vertex to light in cam space
    // g_lgtPos_cam = v_lgtpos_cam[i]; // Position of light in cam space
    // g_camDir_tan = v_camDir_tan[i]; // a vector in from vertex to camera in tan space
    // g_lgtDir_tan = v_lgtDir_tan[i]; // a vector from vertex to light in tan space
    g_lgtpos = v_lgtpos[i];

    EmitVertex();
  }
  EndPrimitive();

}

bool cullPrimitive(){

  bool doCull = true;	   // A variable to hold the return value

  vec3 cornerVer;	 // The vertex that is placed in the corner of the triangle
  vec3 edge1;		   // First edge of the triangle
  vec3 edge2;		   // Second edge of the triangle
  vec3 cullNorm;   // The normal of the triangle
  vec3 transformedCullNorm; // The normal in world space
  vec3 cullCam;		 // The vector from the corner to the camera
  float coe;		   // A coefficient holder

  vec3 cornerVec = gl_in[0].gl_Position.xyz;
  edge1 = gl_in[1].gl_Position.xyz - cornerVec;
  edge2 = gl_in[2].gl_Position.xyz - cornerVec;

  cullNorm = normalize(cross(edge2, edge1));
  // normal of triangle in world space
  transformedCullNorm = (model * vec4(cullNorm, 0.0)).xyz;

  // vector from camera to corner point of triangle
  cullCam = normalize(cornerVec - camPos);

  coe = dot(transformedCullNorm, cullCam);

  if(coe > 0.0f){
    //If the dot-product is positive the triangle should not be culled
    doCull = false;
  }

  return doCull;
}
