#version 450

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 camPos;
uniform mat4 model;
// in vec3 v_posWorld[];
//in vec3 v_worldPos[];
in vec3 v_pos[];
in vec3 v_nor[];
in vec2 v_uvs[];
in vec4 v_lgtpos[];

// normal mapping
in vec3 v_nor_world[]; // normal in world space
in vec3 v_tan_world[]; // tangent in world space
in vec3 v_btan_world[]; // bitangent in world space

out vec3 g_nor_world; // normal in world space
out vec3 g_tan_world; // tangent in world space
out vec3 g_btan_world; // bitangent in world space
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
    g_nor_world = v_nor_world[i];  // normal in world space
    g_tan_world = v_tan_world[i];  // tangent in world space
    g_btan_world = v_btan_world[i]; // bitangent in world space
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
  vec3 cullCam;		 // The vector from the corner to the worldera
  float coe;		   // A coefficient holder

  //vec3 cornerVec = gl_in[0].gl_Position.xyz;
  //edge1 = gl_in[1].gl_Position.xyz - cornerVec;
  //edge2 = gl_in[2].gl_Position.xyz - cornerVec;

  vec3 cornerVec = v_pos[0];
  edge1 = v_pos[2] - cornerVec;
  edge2 = v_pos[1] - cornerVec;

  cullNorm = normalize(cross(edge2, edge1));
  // normal of triangle in world space
  transformedCullNorm = (model * vec4(cullNorm, 0.0)).xyz;

  // vector from worldera to corner point of triangle
  cullCam = normalize(cornerVec - camPos);                  //Done in World Space
  //cullCam = normalize(cornerVec - vec3(0.0, 0.0, 0.0));   //Done in MVP Space

  coe = dot(transformedCullNorm, cullCam);

  if(coe > 0.0f){
    //If the dot-product is positive the triangle should not be culled
    doCull = false;
  }

  return doCull;
}
