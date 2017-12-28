#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include <GL/glew.h>

enum TextureEnums {
  POS_TEX,
  NOR_TEX,
  DIFFSPEC_TEX,
  NUM_OF_TEX
};

void Bind2DTextureTo(GLuint texture_id, TextureEnums target_unit) {
  glActiveTexture(GL_TEXTURE0 + target_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}


#endif
