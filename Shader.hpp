#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader
{
private:
  //GLuint m_vs;
  //GLuint m_gs;
  //GLuint m_fs;

  GLuint m_program;

  //const char* PathToGlsl(const char* path);

  void MakeShader(GLuint id, const char* path);
  void LinkProgram();

public:
  Shader(const char* cs_path);
  Shader(const char* vs_path, const char* fs_path);
  Shader(const char* vs_path, const char* gs_path, const char* fs_path);
  ~Shader();

  GLuint GetProgram();
};


#endif  //SHADER_H
