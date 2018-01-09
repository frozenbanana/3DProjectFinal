#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include <GL/glew.h>

enum TextureEnums {
  POS_TEX,      //0
  NOR_TEX,      //1
  DIFFSPEC_TEX, //2
  MESHDIFF_TEX, //3
  MESHSPEC_TEX, //4
  COMPUTE_TEX,  //5
  //COMPOUT_TEX,
  NUM_OF_TEX
};

inline void Bind2DTextureTo(GLuint texture_id, TextureEnums target_unit) {
  glActiveTexture(GL_TEXTURE0 + target_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

#endif
