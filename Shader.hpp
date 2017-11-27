#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "Transform.hpp"
#include "PntLight.hpp"
#include "DirLight.hpp"
#include "SptLight.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
  struct PntLightUniforms {
    std::vector<GLuint> s_uni_pos;
    std::vector<GLuint> s_uni_amb;
    std::vector<GLuint> s_uni_dif;
    std::vector<GLuint> s_uni_spe;
  };

  struct DirLightUniforms {
    std::vector<GLuint> s_uni_dir;
    std::vector<GLuint> s_uni_amb;
    std::vector<GLuint> s_uni_dif;
    std::vector<GLuint> s_uni_spe;
  };

  struct SptLightUniforms {
    std::vector<GLuint> s_uni_pos;
    std::vector<GLuint> s_uni_amb;
    std::vector<GLuint> s_uni_dif;
    std::vector<GLuint> s_uni_spe;

    //MATRIX UNIFORM?
  };

  GLuint m_program;
  std::vector<GLint> m_matrixUniforms;
  PntLightUniforms m_pnt_lights;
  DirLightUniforms m_dir_lights;
  SptLightUniforms m_spt_lights;

  void MakeShader(GLuint id, const char* path);
  void LinkProgram();

public:
  Shader(const char* cs_path);
  Shader(const char* vs_path, const char* fs_path);
  Shader(const char* vs_path, const char* gs_path, const char* fs_path);
  ~Shader();


  GLuint GetProgram();
  GLint GetUniformArrProp(std::string shader_arr_name, int shader_arr_index, std::string shader_prop);

  //Find functions check if the is a uniform variable and if there is saves it to
  //a variable in shader
  void FindUniformMatrixLoc(std::string uniformName);
  void FindUniformPntLightLoc(std::string shader_arr_name, int shader_arr_index);
  void FindUniformDirLightLoc(std::string shader_arr_name, int shader_arr_index);
  void FindUniformSptLightLoc(std::string shader_arr_name, int shader_arr_index);

  // void UploadMatrix(const Transform& transform, const glm::mat4 view, const glm::mat4 perspective);
  void UploadMatrix(glm::mat4 matrix, GLuint index);
  void UploadPntLight(PntLight in_light, GLuint index);
  void UploadDirLight(DirLight in_light, GLuint index);
  void UploadSptLight(SptLight in_light, GLuint index);

};


#endif  //SHADER_H
