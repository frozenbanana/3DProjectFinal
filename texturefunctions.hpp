#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include <GL/glew.h>
#include <iostream>
//#include <stdio.h>

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
  //std::cout << "Binding texture id: " << texture_id << " to texture unit: " << target_unit << '\n';
  glActiveTexture(GL_TEXTURE0 + target_unit);
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

//From the interwebs: http://www.lighthouse3d.com/cg-topics/error-tracking-in-opengl/
//-----------------------------------------------------------------
// Print for OpenGL errors
//
// Returns 1 if an OpenGL error occurred, 0 otherwise.
//
//NTS:  When funct is called it checks the error flag, outputs its value and then
//      resets it to [no error]. This means it checks the earliest error


#define printOpenGLError() printOglError(__FILE__, __LINE__)
inline int printOglError(char *file, int line)
{

    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n",
			     file, line, gluErrorString(glErr));
        retCode = 1;
    }
    return retCode;
}

#endif
