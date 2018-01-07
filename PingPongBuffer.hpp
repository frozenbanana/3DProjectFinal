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

  void createBuffer(GLuint* buffer_id);
  void bindAndCompute(GLuint source_buffer, GLuint target_buffer);
public:
  PingPongBuffer(int texture_width, int texture_height);
  ~PingPongBuffer();

  void InitPingPongBuffer();

  //OLD STUFF
  //void pingpongRender(JMshader* shaderPtr, int numOfPasses, GLuint sourceBuffer);
	//
	//void getUniformSamplerLocation(JMshader* shaderPtr);
	//void uploadUniformSampler(JMshader* shaderPtr);
	//
	//void locateUploadAndBindUniformSampler(JMshader* shaderPtr);

	//void getUniformVec2Location(JMshader* shaderPtr);
	//
	//void bindMap();
};

#endif
