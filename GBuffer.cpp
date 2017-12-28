//THIS ONE

#include "GBuffer.hpp"

#include "GLOBALS.hpp"
#include "texturefunctions.hpp"

//Private

void GBuffer::CreateTexture(GLuint id, GLint internalFormat, GLenum format, GLenum type) {
  //Bind it as a 2D texture
  glBindTexture(GL_TEXTURE_2D, id);
  //Define some data about the texture
  glTexImage2D(
    GL_TEXTURE_2D,    //GLenum target         -The type of the texture
    0,                //GLint level           -Level of detail
    internalFormat,   //GLint internalFormat  -How the texture saves data
    WINDOW_WIDTH,     //GLsizei width         -Width of texture
    WINDOW_HEIGHT,    //GLsizei height        -Height of texture
    0,                //GLint border          -"This value must be 0"...
    format,           //GLenum format         -How each pixel should be read
    type,             //GLenum type           -What format each pixel is representing its data in
    NULL              //const GLvoid* data    -A pointer to where the image data lies
  );
  //Set mipmap levels
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void GBuffer::AttachTexture(GLuint id, GLenum attachment) {
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

GBuffer::GBuffer() {

}

GBuffer::~GBuffer() {

}

void GBuffer::InitGBuffer() {
    /*########################################################################*/
    /*############ REMEMBER TO INITIALIZE GLEW BEFORE CALLING ################*/
    /*########################################################################*/

    //Generate a framebuffer and bind it as a framebuffer
    glGenFramebuffers(1, &(this->m_gBuffer));
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_gBuffer);

    //NTS: glGenTextures takes a reference. This is hadled outside of 'CreateTexture()'
    //due to the reference otherwise binding to the in-scope temp variable 'id'.

    //Create and attach texture for positions
    glGenTextures(1, &(this->m_gPosition));
    this->CreateTexture(this->m_gPosition, GL_RGB16F, GL_RGB, GL_FLOAT);
    this->AttachTexture(this->m_gPosition, GL_COLOR_ATTACHMENT0);

    //----
    /*
    glBindTexture(GL_TEXTURE_2D, this->m_gPosition);
    //Define some data about the texture
    glTexImage2D(
      GL_TEXTURE_2D,    //GLenum target         -The type of the texture
      0,                //GLint level           -Level of detail
      GL_RGB16F,//internalFormat,   //GLint internalFormat  -How the texture saves data
      WINDOW_WIDTH,     //GLsizei width         -Width of texture
      WINDOW_HEIGHT,    //GLsizei height        -Height of texture
      0,                //GLint border          -"This value must be 0"...
      GL_RGB,//format,           //GLenum format         -How each pixel should be read
      GL_FLOAT,//type,             //GLenum type           -What format each pixel is representing its data in
      NULL              //const GLvoid* data    -A pointer to where the image data lies
    );
    //Set mipmap levels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,       //GLenum target     -Specify target as the framebuffer
      GL_COLOR_ATTACHMENT0,//attachment,           //GLenum attachment -The point where a texture shall be attatched
      GL_TEXTURE_2D,        //GLenum textarget  -Specifies the type of the texture
      this->m_gPosition,//id,                   //GLuint texture    -The point where the texture lies
      0                     //GLint level       -Mipmap level, "Must be 0"...
    );
    */
    //----

    //Create and attach texture for normals
    glGenTextures(1, &(this->m_gNormal));
    this->CreateTexture(this->m_gNormal, GL_RGB16F, GL_RGB, GL_FLOAT);
    this->AttachTexture(this->m_gNormal, GL_COLOR_ATTACHMENT1);

    //Create and attach texture for diffuse and specular color
    glGenTextures(1, &(this->m_gDiffSpec));
    this->CreateTexture(this->m_gDiffSpec, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
    this->AttachTexture(this->m_gDiffSpec, GL_COLOR_ATTACHMENT2);

    //Tell which attachments in the framebuffer that are relevant
    GLuint att_arr[3] = {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, att_arr);

    //Create a depth buffer and bind it
    glGenRenderbuffers(1, &(this->m_rbo_depth));
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_rbo_depth);
    //MRMRMRMMR
    glRenderbufferStorage(
      GL_RENDERBUFFER,      //GLenum target         -Specify binding target
      GL_DEPTH_COMPONENT,   //GLenum internalFormat -The format which the buffer stores (here: it's the preset for depth values)
      WINDOW_WIDTH,         //GLsizei width         -Width of buffer
      WINDOW_HEIGHT         //GLsizei height        -Height of buffer
    );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_rbo_depth);

    //Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "ERROR::GBUFFER::FRAMEBUFFER::INCOMPLETE" << std::endl;
    }

    //Then unbind the frame buffer. This ensures that the buffer is not used or (more importantly) altered by mistake. Explicitly call a function when use is wanted.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //And then set a clear color
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void GBuffer::PrepGeoPass() {
  //Bind this frame buffer as the active buffer
  glBindFramebuffer(GL_FRAMEBUFFER, this->m_gBuffer);
  //Buffer should be unbound after render
}

void GBuffer::PrepLightPass() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //NTS: Bind2DTextureTo() lies in texturefunctions.hpp
  Bind2DTextureTo(this->m_gPosition, POS_TEX);

  Bind2DTextureTo(this->m_gNormal, NOR_TEX);

  Bind2DTextureTo(this->m_gDiffSpec, DIFFSPEC_TEX);
}

void GBuffer::FindUniformSamplerLocs(GLuint shader_program_id) {

  this->m_uni_gPosition = glGetUniformLocation(shader_program_id, "gPosition" );
  if (this->m_uni_gPosition == (GLuint)-1){
    //Error
    std::cout << "ERROR::GBUFFER::SAMPLER_UNIFORM::gPosition_NOT_FOUND" << '\n';
  }
  this->m_uni_gNormal   = glGetUniformLocation(shader_program_id, "gNormal"   );
  if (this->m_uni_gNormal == (GLuint)-1){
    //Error
    std::cout << "ERROR::GBUFFER::SAMPLER_UNIFORM::gNormal_NOT_FOUND" << '\n';
  }
  this->m_uni_gDiffSpec = glGetUniformLocation(shader_program_id, "gDiffSpec" );
  if (this->m_uni_gDiffSpec == (GLuint)-1){
    //Error
    std::cout << "ERROR::GBUFFER::SAMPLER_UNIFORM::gDiffSpec_NOT_FOUND" << '\n';
  }
}

void GBuffer::UploadUniformSamplers() {
  //Note: Before calling, ensure right shader is being used

  glUniform1i(this->m_uni_gPosition, POS_TEX);
  glUniform1i(this->m_uni_gNormal, NOR_TEX);
  glUniform1i(this->m_uni_gDiffSpec, DIFFSPEC_TEX);
}
