#ifndef LBUFFER_H
#define LBUFFER_H

#include <GL/glew.h>

class LBuffer
{
private:
  //
  GLuint m_lBuffer;
  GLuint m_lDepth;

  GLuint m_uni_lDepth;

  void CreateTexture(GLuint id, GLint internalFormat, GLenum format, GLenum type);
  void AttachTexture(GLuint id, GLenum attachment);
public:
  LBuffer();
  ~LBuffer();

  void InitLBuffer();
  void PrepShadowPass();
  void PrepLightPass();

  void FindUniformSamplerLocs(GLuint shader_program_id);
  void UploadUniformSamplers();

  GLuint GetShaTextureId();
};

#endif
