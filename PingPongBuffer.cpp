#include "PingPongBuffer.hpp"

#include "texturefunctions.hpp"

//Private
void PingPongBuffer::createBuffer(GLuint buffer_id) {
  //Bind2DTextureTo(buffer_id, COMPUTE_TEX);
  glBindTexture(GL_TEXTURE_2D, buffer_id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,				          //Mipmaps
		GL_RGBA,		        //Internal format		//GL_RGB16F
    this->m_texture_width,
    this->m_texture_height,
		0,				          //Frontier stuff
		GL_RGBA,		        //Format read			//GL_RGB
		GL_UNSIGNED_BYTE,		//Type of values in read format		//GL_FLOAT
		NULL			          //source
	);

  //testing : https://www.khronos.org/opengl/wiki/Common_Mistakes#Creating_a_complete_texture
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  //testing : NTS: nope

  //glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//When shrunk go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//When enlarged go blurry
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//Set wrapping to clamp to edge

  //glBindTexture(GL_TEXTURE_2D, 0);
}

void PingPongBuffer::bindAndCompute(GLuint source_buffer, GLuint target_buffer) {
  glBindImageTexture(
    0,                  //Always bind to slot 0
    source_buffer,
    0,
    GL_FALSE,
    0,
    GL_READ_ONLY,			  //Only read from this texture
    GL_RGBA8						//GL_RGB16F
  );

  glBindImageTexture(
    1,                  //Always bind to slot 1
    target_buffer,
    0,
    GL_FALSE,
    0,
    GL_WRITE_ONLY,			//Only write to this texture
    GL_RGBA8						//GL_RGB16F
  );

  glDispatchCompute(this->m_texture_width / 10, this->m_texture_height / 10, 1);
  //glDispatchCompute(1, this->m_texture_height, 1);			//Call upon shader
  //glDispatchCompute(this->m_texture_width, 1, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}



//Public
PingPongBuffer::PingPongBuffer() {

}

PingPongBuffer::~PingPongBuffer() {

}

void PingPongBuffer::InitPingPongBuffer(int texture_width, int texture_height, GLint xy_uniform_id) {
  this->m_texture_width = texture_width;
  this->m_texture_height = texture_height;

  this->m_xy_uniLoc = xy_uniform_id;

  glGenTextures(2, this->m_buffers);

  this->createBuffer(this->m_buffers[0]);
  this->createBuffer(this->m_buffers[1]);

  //EX
  this->CreateQuad();

}

void PingPongBuffer::DoPingPong(int n_passes, GLuint src_buffer) {

  int n = n_passes * 2;			//Each iteration of the blur includes both x and y axis
                                //Also ensures that pingpongBuffer[0] is allways written to last
  int x = 1;
  int y = 0;
  int t;		                    //Holder variable

  //Do a first pass if there are supposed to be passes at all
  if(n_passes > 0){
    this->bindAndCompute(src_buffer, this->m_buffers[1]);
  }

  for (int i = 1; i < n; i++) {								                                  //Loop starts at 1 as the first pass has been done

    glUniform2i(this->m_xy_uniLoc, x, y);		                                    //Update uniform vector

    this->bindAndCompute(this->m_buffers[x], this->m_buffers[y]);	              //Send in alternating buffers

    //Swap so x = 0 or 1
    //and y = 1 or 0
    t = x;
    x = y;
    y = t;

  }

}

void PingPongBuffer::BindResult() {
  Bind2DTextureTo(this->m_buffers[0], COMPUTE_TEX);
  //glBindImageTexture(
  //  COMPUTE_TEX,        //Always bind to slot 7
  //  this->m_buffers[0],
  //  0,
  //  GL_FALSE,
  //  0,
  //  GL_READ_ONLY,			  //Only read from this texture
  //  GL_RGBA8						//GL_RGB16F
  //);
}

//EXPERIMENT STUFF
//Private
void PingPongBuffer::CreateQuad() {
  //Create four vertices covering the window
  float quad[] = {
    //Pos                 //UVs
    -1.0f,  1.0f, 1.0f,   0.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,   0.0f, 0.0f,
     1.0f,  1.0f, 1.0f,   1.0f, 1.0f,
     1.0f, -1.0f, 1.0f,   1.0f, 0.0f
  };

  //Generate arrays and buffers
  glGenVertexArrays(1, &(this->m_quadVAO));
  glGenBuffers(1, &(this->m_quadVBO));

  //Bind them together
  glBindVertexArray(this->m_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_quadVBO);

  //Tell GL how to handle the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);

  //Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,                  //Index of the attribute
    3,                  //Number of components for the attribute
    GL_FLOAT,           //Type of values in attribute
    GL_FALSE,           //Are they normalized?
    5 * sizeof(float),  //In the array, how long steps should be taken for each element?
    (void*)0          //Inside the element, how big is the offset to the relevant values?
  );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,                          //Index of the attribute
    2,                          //Number of components for the attribute
    GL_FLOAT,                   //Type of values in attribute
    GL_FALSE,                   //Are they normalized?
    5 * sizeof(float),          //In the array, how long steps should be taken for each element?
    (void*)(3 * sizeof(float))  //Inside the element, how big is the offset to the relevant values?
  );
}

void PingPongBuffer::RenderQuad() {
  glBindVertexArray(this->m_quadVAO);     //Bind the quad
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  //Draw the stuff
  glBindVertexArray(0);                   //Unbind the quad
}

void PingPongBuffer::bindAndPixel(GLuint source_buffer, GLuint target_buffer) {
  glBindImageTexture(
    0,                  //Always bind to slot 0
    source_buffer,
    0,
    GL_FALSE,
    0,
    GL_READ_ONLY,			  //Only read from this texture
    GL_RGBA8						//GL_RGB16F
  );

  glBindImageTexture(
    1,                  //Always bind to slot 1
    target_buffer,
    0,
    GL_FALSE,
    0,
    GL_WRITE_ONLY,			//Only write to this texture
    GL_RGBA8						//GL_RGB16F
  );

  this->RenderQuad();
}

//Public
void PingPongBuffer::DoPixel(GLuint src_buffer) {
  this->bindAndPixel(src_buffer, this->m_buffers[0]); //NTS: Bind [0] directly
}
