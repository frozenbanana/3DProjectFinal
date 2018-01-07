#include "PingPongBuffer.hpp"

//Private
void createBuffer(GLuint* buffer_id) {
  glBindTexture(GL_TEXTURE_2D, *buffer);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,				          //Mipmaps
		GL_RGBA,		        //Internal format		//GL_RGB16F
		this->width,
		this->height,
		0,				          //Frontier stuff
		GL_RGBA,		        //Format read			//GL_RGB
		GL_UNSIGNED_BYTE,		//Type of values in read format		//GL_FLOAT
		NULL			          //source
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//When shrunk go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//When enlarged go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
}

void bindAndCompute(GLuint source_buffer, GLuint target_buffer) {
  glBindImageTexture(
    0,                  //Always bind to slot 0
    sourceBuffer,
    0,
    GL_FALSE,
    0,
    GL_READ_ONLY,			  //Only read from this texture
    GL_RGBA8						//GL_RGB16F
  );

  glBindImageTexture(
    1,                  //Always bind to slot 1
    targetBuffer,
    0,
    GL_FALSE,
    0,
    GL_WRITE_ONLY,			//Only write to this texture
    GL_RGBA8						//GL_RGB16F
  );

  glDispatchCompute(1, this->height, 1);			//Call upon shader 
  //glDispatchCompute(this->width, 1, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}



//Public
PingPongBuffer::PingPongBuffer(int texture_width, int texture_height) {
  this->m_texture_width = texture_width;
  this->m_texture_height = texture_height;
}

PingPongBuffer::~PingPongBuffer() {

}

PingPongBuffer::InitPingPongBuffer() {
  glGenTextures(2, this->m_buffers);

  this->createBuffer(&(this->buffers[0]));
  this->createBuffer(&(this->buffers[1]));

}
