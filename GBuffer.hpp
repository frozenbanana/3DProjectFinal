//THIS ONE

#ifndef GBUFFER_H
#define GBUFFER_H

#include <GL/glew.h>

class GBuffer
{
private:
  GLuint m_gBuffer;
  GLuint m_gPosition, m_gNormal, m_gDiffSpec;
  GLuint m_rbo_depth;
  GLuint m_uni_gPosition, m_uni_gNormal, m_uni_gDiffSpec;

  void CreateTexture(GLuint id, GLint internalFormat, GLenum format, GLenum type);
  void AttachTexture(GLuint id, GLenum attachment);
public:
  GBuffer();
  ~GBuffer();

  void InitGBuffer();
  void PrepGeoPass();
  void PrepLightPass();

  void FindUniformSamplerLocs(GLuint shader_program_id);
  void UploadUniformSamplers();
};

#endif
