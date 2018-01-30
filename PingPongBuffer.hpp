#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H

#include <GL/glew.h>

class PingPongBuffer
{
private:
  GLuint m_buffers[2];
  int m_texture_width;
  int m_texture_height;
  //? GLuint quadVAO;
  //? GLuint quadVBO;
  // GLuint ...
  // GLuint ...
  GLint m_xy_uniLoc;

  void createBuffer(GLuint buffer_id);
  void bindAndCompute(GLuint source_buffer, GLuint target_buffer);

  //EXPERIMENT STUFF
  GLuint m_quadVAO;
  GLuint m_quadVBO;

  void CreateQuad();
  void RenderQuad();
  void bindAndPixel(GLuint source_buffer, GLuint target_buffer);

public:
  PingPongBuffer();
  ~PingPongBuffer();

  void InitPingPongBuffer(int texture_width, int texture_height, GLint xy_uniform_id);

  void DoPingPong(int n_passes, GLuint src_buffer);

  void BindResult();
  //OLD STUFF//
	//void getUniformSamplerLocation(JMshader* shaderPtr);
	//void uploadUniformSampler(JMshader* shaderPtr);
	//
	//void locateUploadAndBindUniformSampler(JMshader* shaderPtr);

	//void getUniformVec2Location(JMshader* shaderPtr);
	//
	//void bindMap();

  //EXPERIMENT STUFF
  void DoPixel(GLuint src_buffer);
};

#endif
