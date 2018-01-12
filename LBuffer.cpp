#include "LBuffer.hpp"

#include "GLOBALS.hpp"
#include "texturefunctions.hpp"

//Private

void LBuffer::CreateTexture(GLuint id, GLint internalFormat, GLenum format, GLenum type) {
  //Bind it as a 2D texture
  glBindTexture(GL_TEXTURE_2D, id);
  //Define some data about the texture
  glTexImage2D(
    GL_TEXTURE_2D,    //GLenum target         -The type of the texture
    0,                //GLint level           -Level of detail
    internalFormat,   //GLint internalFormat  -How the texture saves data
    SHADOW_WIDTH,     //GLsizei width         -Width of texture
    SHADOW_HEIGHT,    //GLsizei height        -Height of texture
    0,                //GLint border          -"This value must be 0"...
    format,           //GLenum format         -How each pixel should be read
    type,             //GLenum type           -What format each pixel is representing its data in
    NULL              //const GLvoid* data    -A pointer to where the image data lies
  );

  //
  //These lines of code makes it so that anything outside the range of the depthmap is set to
  //give a value 1.0, that is to say, as far away as possible. This prevents some sharp edges in the shadow
  //It also prevents the texture from wrapping and creating clones of the shadow here and there
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);			//Clamp S-axis close by border (s-axis = x-axis)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);			//Clamp T-axis close by border (t-axis = y-axis)
  GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  //

  //Set mipmap levels
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void LBuffer::AttachTexture(GLuint id, GLenum attachment) {
  //Attach the texture as a colorattachment to the framebuffer
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,       //GLenum target     -Specify target as the framebuffer
    attachment,           //GLenum attachment -The point where a texture shall be attatched
    GL_TEXTURE_2D,        //GLenum textarget  -Specifies the type of the texture
    id,                   //GLuint texture    -The point where the texture lies
    0                     //GLint level       -Mipmap level, "Must be 0"...
  );
}

//Public

LBuffer::LBuffer() {

}

LBuffer::~LBuffer() {

}

void LBuffer::InitLBuffer() {
    /*########################################################################*/
    /*############ REMEMBER TO INITIALIZE GLEW BEFORE CALLING ################*/
    /*########################################################################*/

    //Generate a framebuffer and bind it as a framebuffer
    glGenFramebuffers(1, &(this->m_lBuffer));
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_lBuffer);

    //Create and attach texture for positions
    glGenTextures(1, &(this->m_lDepth));
    this->CreateTexture(this->m_lDepth, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    this->AttachTexture(this->m_lDepth, GL_DEPTH_ATTACHMENT);

    //Tell gl not expect any color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "ERROR::LBUFFER::FRAMEBUFFER::INCOMPLETE" << std::endl;
    }

    //Then unbind the frame buffer. This ensures that the buffer is not used or (more importantly) altered by mistake. Explicitly call a function when use is wanted.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //And then set a clear color
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void LBuffer::PrepShadowPass() {
  //Bind this frame buffer as the active buffer
  glBindFramebuffer(GL_FRAMEBUFFER, this->m_lBuffer);
  //Buffer should be unbound after render
}

void LBuffer::PrepLightPass() {
  //NTS: Bind2DTextureTo() lies in texturefunctions.hpp
  Bind2DTextureTo(this->m_lDepth, DEPTH_TEX);

}

void LBuffer::FindUniformSamplerLocs(GLuint shader_program_id) {
  this->m_uni_lDepth = glGetUniformLocation(shader_program_id, "lDepth" );
  if (this->m_uni_lDepth == (GLuint)-1){
    //Error
    std::cout << "ERROR::LBUFFER::SAMPLER_UNIFORM::lDepth_NOT_FOUND" << '\n';
  }
}

void LBuffer::UploadUniformSamplers() {
  //Note: Before calling, ensure right shader is being used
  glUniform1i(this->m_uni_lDepth, DEPTH_TEX);
}

GLuint LBuffer::GetShaTextureId() {
  return this->m_lDepth;
}
