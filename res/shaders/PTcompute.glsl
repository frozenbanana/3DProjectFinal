#version 450
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_image_load_store : enable

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

layout (local_size_x=10 , local_size_y=10 , local_size_z=1) in;			//Local work-group size

shared vec4 colMap[10][10]; //Positions within one global work group and between the local work groups

uniform ivec2 xORy;														//A variable deciding if blur is made vertically or horizontally

//uniform float weight[5] = float[] (0.38774, 0.24477, 0.24477, 0.06136, 0.06136);		//Gaussian curve numbers [Declining]
uniform float weight[5] = float[] (0.06136, 0.24477, 0.38774, 0.24477, 0.06136);		//Gaussian curve numbers [Spread]

void main() {
  ivec2 txlPos;		//A variable keeping track of where on the texture current texel is from

  ivec2 locPos;

  vec4 result = vec4(0.0, 0.0, 0.0, 0.0);		//A variable to store color
  //vec2 offset;		//A vector to tell where new values should be gotten from
  //ivec2 plus_offset;
  //ivec2 minus_offset;
  //float weight[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

  txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos

  //The index for the shared colMap variable can be determined using
  //gl_LocalInvocationID which gives coordinates between (0,0) and (9, 9)
  locPos = ivec2(gl_LocalInvocationID.xy);


  colMap[locPos.x][locPos.y] = imageLoad(texture_source0, txlPos);

  barrier();

  int cr = int( dot(locPos, xORy) );

  for (int i = -2; i < 3; i++) {

    int index = min( max(cr + i , 0) , 10 );

    result += colMap[index][locPos.y] * weight[i+2] * xORy.x;
    result += colMap[locPos.x][index] * weight[i+2] * xORy.y;
  }

  result = vec4(vec2(gl_GlobalInvocationID.xy) / vec2(640,480), 0.0, 1.0);

  imageStore(texture_target0, txlPos, result);			//Save color in target texture
}
