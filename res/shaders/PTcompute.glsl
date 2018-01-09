#version 440
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_image_load_store : enable

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

uniform vec2 xORy;														//A variable deciding if blur is made vertically or horizontally

uniform float weight[5] = float[] (0.2f, 0.1f, 0.1f, 0.05f, 0.05f);		//Gaussian curve numbers, or at least supposed to be

layout (local_size_x=640 , local_size_y=1 , local_size_z=1) in;			//Local work-group size

void main() {
  ivec2 txlPos;		//A variable keeping track of where on the texture current texel is from
  vec4 result;		//A variable to store color
  vec2 offset;		//A vector to tell where new values should be gotten from
  ivec2 plus_offset;
  ivec2 minus_offset;
  //float weight[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

  txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos

  result = imageLoad(texture_source0, txlPos);			//Get color value

  barrier();

  for(int i = 1; i < 5; i++){
  	offset = ivec2(txlPos.x * i, txlPos.y * i);

  	offset = dot(offset, xORy) * xORy;
  	//Instead of sending in an boolean, send in an vec2 with one 1 and one 0.
  	//Dot multiply the position by that vector to eliminate the term not needed
  	//Scalar multiply the dot product with the axis vector to
  	//get a vector with the offset in either x or y and 0 in the other

  	plus_offset = txlPos + ivec2(offset);
  	minus_offset = txlPos - ivec2(offset);

    result += imageLoad(texture_source0, plus_offset) * weight[i];		//NTS: Fetching a value outside the perimeter of the texture
  	result += imageLoad(texture_source0, minus_offset) * weight[i];		//will return only zeroes, additivity will cancel out any fault
  }

  //result = vec4(vec2(gl_GlobalInvocationID.xy) / vec2(640,480), 0.0, 1.0);

  imageStore(texture_target0, txlPos, result);			//Save color in target texture
}

// void main(){
// 	ivec2 txlPos;		//A variable keeping track of where on the texture current texel is from
// 	vec4 result;		//A variable to store color
// 	vec2 offset;		//A vector to tell where new values should be gotten from
// 	ivec2 plus_offset;
// 	ivec2 minus_offset;
// 	//float weight[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
//
// 	txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos
//
// 	result = imageLoad(sourceTex, txlPos);			//Get color value
//
// 	barrier();										//Ensure reading has finished before writing
//
// 	for(int i = 1; i < 5; i++){
// 		offset = ivec2(txlPos.x * i, txlPos * i);
//
// 		offset = dot(txlPos, xORy) * xORy;
// 		//Instead of sending in an boolean, send in an vec2 with one 1 and one 0.
// 		//Dot multiply the position by that vector to eliminate the term not needed
// 		//Scalar multiply the dot product with the axis vector to
// 		//get a vector with the offset in either x or y and 0 in the other
//
//
// 		plus_offset = txlPos + ivec2(offset);
// 		minus_offset = txlPos - ivec2(offset);
//
// 		result += imageLoad(sourceTex, plus_offset) * weight[i];		//NTS: Fetching a value outside the perimeter of the texture
// 		result += imageLoad(sourceTex, minus_offset) * weight[i];		//will return only zeroes, additivity will cancel out any fault
//
// 		//result += 0.1 * weight[4];
// 	}
//
//
// 	imageStore(targetTex, txlPos, result);			//Save color in target texture
//
// 	//imageStore(targetTex, txlPos, vec4(0.5f));	//Now gives result!
// }
