#include "Shader.hpp"

#include <iostream>
#include <string>

#include <fstream>
#include <streambuf>
#include <string> //to_string();

//Private

void Shader::MakeShader(GLuint id, const char* path) {

  //----------------------------------------------------------------------------
  std::ifstream glsl_file(path);

  if (!(glsl_file.is_open())) {
    std::cout << "ERROR::FILE::PATH::COULD_NOT_BE_OPENED\n" << path << std::endl;
  }

  std::string glsl_text((std::istreambuf_iterator<char>(glsl_file)), std::istreambuf_iterator<char>());
  glsl_file.close();

  const char* glsl_data = glsl_text.c_str();
  //----------------------------------------------------------------------------

  glShaderSource(id, 1, &glsl_data, NULL);
  glCompileShader(id);

  // Check if shader where compiled fine
  int  success;
  char infoLog[512];

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if(!success) {
      GLint type;
      glGetShaderiv(id, GL_SHADER_TYPE, &type);
      std::string type_name = "";

      switch (type) {
        case GL_VERTEX_SHADER:
          type_name = "VERTEX";
          break;
        case GL_GEOMETRY_SHADER:
          type_name = "GEOMETRY";
          break;
        case GL_FRAGMENT_SHADER:
          type_name = "FRAGMENT";
          break;
        default:
          type_name = "UNKNOWN";
          break;
      }

      glGetShaderInfoLog(id, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::" << type_name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

void Shader::LinkProgram() {
  glLinkProgram(this->m_program);

  int  success;
  char infoLog[512];

  glGetProgramiv(this->m_program, GL_LINK_STATUS, &success);
  if(!success) {
      glGetShaderInfoLog(this->m_program, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
}

//Public
Shader::Shader(const char* cs_path) {
  //WIP
}

Shader::Shader(const char* vs_path, const char* fs_path) {
  GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
  this->MakeShader(vs_id, vs_path);

  GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
  this->MakeShader(fs_id, fs_path);

  this->m_program = glCreateProgram();
  glAttachShader(this->m_program, vs_id);
  glAttachShader(this->m_program, fs_id);
  this->LinkProgram();
}

Shader::Shader(const char* vs_path, const char* gs_path, const char* fs_path) {
  GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
  this->MakeShader(vs_id, vs_path);

  GLuint gs_id = glCreateShader(GL_GEOMETRY_SHADER);
  this->MakeShader(gs_id, gs_path);

  GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
  this->MakeShader(fs_id, fs_path);

  this->m_program = glCreateProgram();
  glAttachShader(this->m_program, vs_id);
  glAttachShader(this->m_program, gs_id);
  glAttachShader(this->m_program, fs_id);
  this->LinkProgram();
}

Shader::~Shader() {

}

GLuint Shader::GetProgram(){
  return this->m_program;
}

GLint Shader::GetUniform(std::string uniform_name) {
  //Function general;

  GLint uniLoc = glGetUniformLocation(this->m_program, uniform_name.c_str());
  if(uniLoc == (GLint)-1) {
    //If it does not, write out an error
    std::cout << "ERROR::SHADER::" << uniform_name << "::UNIFORM_NOT_FOUND" << std::endl;
  }

  return uniLoc;
}

GLint Shader::GetUniformArr(std::string shader_arr_name, int shader_arr_index) {
  //Function general

  //Create a line of code calling an array of uniforms and pick an index
  std::string uniformName = shader_arr_name + "[" + std::to_string(shader_arr_index) + "]";

  //Now check if it exists
  GLint uniLoc = glGetUniformLocation(this->m_program, uniformName.c_str());
  if(uniLoc == (GLint)-1) {
    //If it does not, write out an error
    std::cout << "ERROR::SHADER::" << uniformName << "::UNIFORM_NOT_FOUND" << std::endl;
  }

  return uniLoc;
}

GLint Shader::GetUniformArrProp(std::string shader_arr_name, int shader_arr_index, std::string shader_prop) {
  //Function general

  //Create a line of code calling an array of structs and then picking an element in that array
  std::string uniformName = shader_arr_name + "[" + std::to_string(shader_arr_index) + "]." + shader_prop;

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  GLint uniLoc = glGetUniformLocation(this->m_program, uniformName.c_str());
  if(uniLoc == (GLint)-1) {
    //If it does not, write out an error
    std::cout << "ERROR::SHADER::" << uniformName << "::UNIFORM_NOT_FOUND" << std::endl;
  }

  return uniLoc;
}

void Shader::DirectInt(std::string uniform_name, int value) {

  GLint uniform_loc = GetUniform(uniform_name);

  if(uniform_loc != (GLint)-1) {
    glUniform1i(uniform_loc, value);
  }

}

void Shader::FindUniformMatrixLoc(std::string uniformName) {

  GLint uniformLoc = glGetUniformLocation(m_program, uniformName.c_str());
  if(uniformLoc == (GLint)-1) {
    std::cout << "ERROR::SHADER::" << uniformName << "::UNIFORM_NOT_FOUND" << std::endl;
  }
  else {
    //std::cout << uniformName << "found\n";
    m_matrixUniforms.push_back(uniformLoc);
    //std::cout << "Pushed " << uniformName << " to index: " << (this->m_matrixUniforms.size() - 1) << '\n';
  }
}

void Shader::FindUniformTextureLoc(std::string texture_type, int texture_num) {
  //NTS: Texture type should be 'diffuse', 'specular', 'compute', etc

  std::string uniform_name = "texture_" + texture_type + std::to_string(texture_num);

  GLint uniform_loc = GetUniform(uniform_name);

  if(uniform_loc != (GLint)-1) {
    this->m_textureUniforms.push_back(uniform_loc);
  }
}

void Shader::FindUniformPntLightLoc(std::string shader_arr_name, int shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check pos ,pnt
  uniLocs[0] = GetUniformArrProp(shader_arr_name, shader_arr_index, "pos");
  //Check amb ,pnt
  uniLocs[1] = GetUniformArrProp(shader_arr_name, shader_arr_index, "amb");
  //Check dif ,pnt
  uniLocs[2] = GetUniformArrProp(shader_arr_name, shader_arr_index, "dif");
  //Check .spe ,pnt
  uniLocs[3] = GetUniformArrProp(shader_arr_name, shader_arr_index, "spe");

  //eIA is true, if any of the statements it is AND:ed with is false it will
  //turn false and this all remaining statements also become false
  for (int i = 0; i < 4; i++) {
    everythingIsAlright = everythingIsAlright && (uniLocs[i] != (GLint)-1);
  }

  //If nothing has gone wrong the uniform locations are saved
  if(everythingIsAlright) {
    this->m_pnt_lights.s_uni_pos.push_back(uniLocs[0]);
    this->m_pnt_lights.s_uni_amb.push_back(uniLocs[1]);
    this->m_pnt_lights.s_uni_dif.push_back(uniLocs[2]);
    this->m_pnt_lights.s_uni_spe.push_back(uniLocs[3]);
  }
}

void Shader::FindUniformDirLightLoc(std::string shader_arr_name, int shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check dir ,dir
  uniLocs[0] = GetUniformArrProp(shader_arr_name, shader_arr_index, "dir");
  //Check amb ,dir
  uniLocs[1] = GetUniformArrProp(shader_arr_name, shader_arr_index, "amb");
  //Check dif ,dir
  uniLocs[2] = GetUniformArrProp(shader_arr_name, shader_arr_index, "dif");
  //Check .spe ,dir
  uniLocs[3] = GetUniformArrProp(shader_arr_name, shader_arr_index, "spe");

  //eIA is true, if any of the statements it is AND:ed with is false it will
  //turn false and this all remaining statements also become false
  for (int i = 0; i < 4; i++) {
    everythingIsAlright = everythingIsAlright && (uniLocs[i] != (GLint)-1);
  }

  //If nothing has gone wrong the uniform locations are saved
  if(everythingIsAlright) {
    this->m_dir_lights.s_uni_dir.push_back(uniLocs[0]);
    this->m_dir_lights.s_uni_amb.push_back(uniLocs[1]);
    this->m_dir_lights.s_uni_dif.push_back(uniLocs[2]);
    this->m_dir_lights.s_uni_spe.push_back(uniLocs[3]);
  }
}

void Shader::FindUniformSptLightLoc(std::string shader_arr_name, int shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check pos ,spt
  uniLocs[0] = GetUniformArrProp(shader_arr_name, shader_arr_index, "pos");
  //Check amb ,spt
  uniLocs[1] = GetUniformArrProp(shader_arr_name, shader_arr_index, "amb");
  //Check dif ,spt
  uniLocs[2] = GetUniformArrProp(shader_arr_name, shader_arr_index, "dif");
  //Check .spe ,spt
  uniLocs[3] = GetUniformArrProp(shader_arr_name, shader_arr_index, "spe");


  //MATRIX

  //eIA is true, if any of the statements it is AND:ed with is false it will
  //turn false and this all remaining statements also become false
  for (int i = 0; i < 4; i++) {
    everythingIsAlright = everythingIsAlright && (uniLocs[i] != (GLint)-1);
  }

  //If nothing has gone wrong the uniform locations are saved
  if(everythingIsAlright) {
    this->m_spt_lights.s_uni_pos.push_back(uniLocs[0]);
    this->m_spt_lights.s_uni_amb.push_back(uniLocs[1]);
    this->m_spt_lights.s_uni_dif.push_back(uniLocs[2]);
    this->m_spt_lights.s_uni_spe.push_back(uniLocs[3]);

    //MATRIX
  }
}

void Shader::UploadMatrix(glm::mat4 matrix, GLuint index) {
  glUniformMatrix4fv(m_matrixUniforms[index], 1, GL_FALSE, glm::value_ptr(matrix));
  //std::cout << "Uploaded index: " << index << '\n';
}

void Shader::UploadTexture(GLuint tex_id, int index) {

  if (this->m_textureUniforms.size() <= (unsigned int)index) {
    std::cout << "ERROR::SHADER::TEXTURE_UNIFORMS::INDEX_OUT_OF_BOUNDS" << '\n';
  }
  else {
    glUniform1i(this->m_textureUniforms[index], tex_id);
  }
}

void Shader::UploadPntLight(PntLight in_light, GLuint index) {
  //NTS: glUniformXfv takes an uniform id, a number and a vecX pointer

  //Upload to pos
  glUniform3fv(this->m_pnt_lights.s_uni_pos[index], 1, glm::value_ptr(in_light.getPos()));
  //Upload to amb
  glUniform4fv(this->m_pnt_lights.s_uni_amb[index], 1, glm::value_ptr(in_light.getAmb()));
  //Upload to dif
  glUniform4fv(this->m_pnt_lights.s_uni_dif[index], 1, glm::value_ptr(in_light.getDif()));
  //Upload to spe
  glUniform4fv(this->m_pnt_lights.s_uni_spe[index], 1, glm::value_ptr(in_light.getSpe()));
}

void Shader::UploadDirLight(DirLight in_light, GLuint index) {
  //NTS: glUniformXfv takes an uniform id, a number and a vecX pointer

  //Upload to dir
  glUniform3fv(this->m_dir_lights.s_uni_dir[index], 1, glm::value_ptr(in_light.getDir()));
  //Upload to amb
  glUniform4fv(this->m_dir_lights.s_uni_amb[index], 1, glm::value_ptr(in_light.getAmb()));
  //Upload to dif
  glUniform4fv(this->m_dir_lights.s_uni_dif[index], 1, glm::value_ptr(in_light.getDif()));
  //Upload to spe
  glUniform4fv(this->m_dir_lights.s_uni_spe[index], 1, glm::value_ptr(in_light.getSpe()));
}

void Shader::UploadSptLight(SptLight in_light, GLuint index) {
  //NTS: glUniformXfv takes an uniform id, a number and a vecX pointer

  //Upload to pos
  glUniform3fv(this->m_spt_lights.s_uni_pos[index], 1, glm::value_ptr(in_light.getPos()));
  //Upload to amb
  glUniform4fv(this->m_spt_lights.s_uni_amb[index], 1, glm::value_ptr(in_light.getAmb()));
  //Upload to dif
  glUniform4fv(this->m_spt_lights.s_uni_dif[index], 1, glm::value_ptr(in_light.getDif()));
  //Upload to spe
  glUniform4fv(this->m_spt_lights.s_uni_spe[index], 1, glm::value_ptr(in_light.getSpe()));

  //MATRIX
}
