#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "Transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
  //GLuint m_vs;
  //GLuint m_gs;
  //GLuint m_fs;

  GLuint m_program;

  //const char* PathToGlsl(const char* path);

  // enum {
  //   MATRIX_U,
  //   VEC3_U,

  //   NUM_OF_U
  // };
  // GLuint m_uniforms[NUM_OF_U];
  std::vector<GLint> m_matrixUniforms;
  void MakeShader(GLuint id, const char* path);
  void LinkProgram();

public:
  Shader(const char* cs_path);
  Shader(const char* vs_path, const char* fs_path);
  Shader(const char* vs_path, const char* gs_path, const char* fs_path);
  ~Shader();

  void GetUniformMatrixLoc(std::string uniformName);
  // void UpdateMatrix(const Transform& transform, const glm::mat4 view, const glm::mat4 perspective);
  void UpdateMatrix(glm::mat4 matrix, GLuint index);

  GLuint GetProgram();
};


#endif  //SHADER_H
