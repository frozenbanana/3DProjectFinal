#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include <GL/glew.h>

enum TextureEnums {
  POS_TEX,        //0
  NOR_TEX,        //1
  DIFFSPEC_TEX,   //2
  LGTPOS_TEX,     //3
  MESHDIFF_TEX,   //4
  MESHSPEC_TEX,   //5
  NORMALMAP_TEX,  //6
  COMPUTE_TEX,    //7
  DEPTH_TEX,      //8
  NUM_OF_TEX
};

inline void Bind2DTextureTo(GLuint texture_id, TextureEnums target_unit) {
  std::cout << "Binding texture id: " << texture_id << " to texture unit: " << target_unit << '\n';
  glActiveTexture(GL_TEXTURE0 + target_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

#endif
