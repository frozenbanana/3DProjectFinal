#version 450

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

in vec2 v_uvs;

void main() {
  ivec2 imgCrds = ivec2( v_uvs.x * 640, v_uvs.y * 480 );
  vec4 out_col = imageLoad(texture_source0, imgCrds);
  //vec4 out_col = vec4(v_uvs, 0.0, 1.0);
  imageStore(texture_target0, imgCrds, out_col);
}//Main
