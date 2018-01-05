#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 camPos;

in vec3 v_posWorld[];
in vec3 v_camPos[];

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
    EmitVertex();
  }
  EndPrimitive();

}

bool cullPrimitive(){

  bool doCull = true;	   //A variable to hold the return value

  vec3 cornerVer;	   //The vertex that is placed in the corner of the triangle
  vec3 edge1;		   //First edge of the triangle
  vec3 edge2;		   //Second edge of the triangle
  vec3 cullNorm;           //The normal of the triangle
  vec3 cullCam;		   //The vector from the corner to the camera
  float coe;		   //A coefficient holder

  cornerVer = vec3(v_posWorld[1]);

  edge1 = vec3(cornerVer - v_posWorld[0]);
  edge2 = vec3(cornerVer - v_posWorld[2]);

  cullNorm = normalize(cross(edge1, edge2));

  cullCam = normalize(cornerVer - camPos);

  coe = dot(cullNorm, cullCam);

  if(coe >= 0.0f){
    //If the dot-product is positive the triangle should not be culled
    doCull = false;
  }

  return doCull;
}
