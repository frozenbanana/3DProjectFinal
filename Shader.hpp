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
    std::vector<GLuint> s_uni_con;
    std::vector<GLuint> s_uni_lin;
    std::vector<GLuint> s_uni_qua;
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
  };

  GLuint m_program;
  std::vector<GLint> m_matrixUniforms;
  std::vector<GLint> m_vec3Uniforms;
  std::vector<GLint> m_textureUniforms;
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
  GLint GetUniform(std::string uniform_name);
  GLint GetUniformArr(std::string shader_arr_name, int shader_arr_index);
  GLint GetUniformArrProp(std::string shader_arr_name, int shader_arr_index, std::string shader_prop);

  //Direct functions take a uniform name and instantly tries to upload the value
  //to the shader
  void DirectInt(std::string uniform_name, int value);

  //Find functions check if the is a uniform variable and if there is saves it to
  //a variable in shader
  void FindUniformVec3Loc(std::string uniformName);
  void FindUniformMatrixLoc(std::string uniformName);
  void FindUniformTextureLoc(std::string texture_type, int texture_num);
  void FindUniformPntLightLoc(std::string shader_arr_name, int shader_arr_index);
  void FindUniformDirLightLoc(std::string shader_arr_name, int shader_arr_index);
  void FindUniformSptLightLoc(std::string shader_arr_name, int shader_arr_index);

  // void UploadMatrix(const Transform& transform, const glm::mat4 view, const glm::mat4 perspective);
  void UploadDirectInt(int i);
  void UploadVec3(glm::vec3 vec, GLuint index);
  void UploadMatrix(glm::mat4 matrix, GLuint index);
  //void UploadTexture(GLuint tex_id, int index);
  void UploadPntLight(PntLight in_light, GLuint index);
  void UploadDirLight(DirLight in_light, GLuint index);
  void UploadSptLight(SptLight in_light, GLuint index);

};


#endif  //SHADER_H
