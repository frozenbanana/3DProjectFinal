//TBA CODE DUMP

void UploadLights(LightPack in_pack) {
  //Upload each type separatly
}

struct PntLightUniforms {
  std::vector<GLuint> s_uni_pos;
  std::vector<GLuint> s_uni_amb;
  std::vector<GLuint> s_uni_dif;
  std::vector<GLuint> s_uni_spe;
}
struct DirLightUniforms {
  std::vector<GLuint> s_uni_dir;
  std::vector<GLuint> s_uni_amb;
  std::vector<GLuint> s_uni_dif;
  std::vector<GLuint> s_uni_spe;
}
struct SptLightUniforms {
  std::vector<GLuint> s_uni_pos;
  std::vector<GLuint> s_uni_amb;
  std::vector<GLuint> s_uni_dif;
  std::vector<GLuint> s_uni_spe;

  //MATRIX UNIFORM?
}
//------------------------------------------------------------------------------
GLint GetUniformArrProp(std::string shader_arr_name, int shader_arr_index, std::string shader_prop) {
  //Function general

  //Create a line of code calling an array of structs and then picking an element in that array
  std::string uniformName = shader_arr_name + "[" + shader_arr_index + "]." + shader_prop;

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  GLint uniLoc = glGetUniformLocation(this->m_program, uniformName.c_str());
  if(uniformLoc == (GLint)-1) {
    //If it does not, write out an error
    std::cout << "ERROR::SHADER::" << uniformName << "::UNIFORM_NOT_FOUND" << std::endl;
  }

  return uniLoc;
}


void FindUniformPntLightLoc(int shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check pos ,pnt
  uniLocs[0] = CheckUniformArrProp("pnt_lights", shader_arr_index, "pos");
  //Check amb ,pnt
  uniLocs[1] = CheckUniformArrProp("pnt_lights", shader_arr_index, "amb");
  //Check dif ,pnt
  uniLocs[2] = CheckUniformArrProp("pnt_lights", shader_arr_index, "dif");
  //Check .spe ,pnt
  uniLocs[3] = CheckUniformArrProp("pnt_lights", shader_arr_index, "spe");

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

void FindUniformDirLightLoc(shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check pos ,pnt
  uniLocs[0] = CheckUniformArrProp("dir_lights", shader_arr_index, "dir");
  //Check amb ,dir
  uniLocs[1] = CheckUniformArrProp("dir_lights", shader_arr_index, "amb");
  //Check dif ,dir
  uniLocs[2] = CheckUniformArrProp("dir_lights", shader_arr_index, "dif");
  //Check .spe ,dir
  uniLocs[3] = CheckUniformArrProp("dir_lights", shader_arr_index, "spe");

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

void FindUniformSptLightLoc(int shader_arr_index) {
  //Function specific for our shaders

  bool everythingIsAlright = true;  //A variable that is set to false if any uniform location is not found
  GLint uniLocs[4];                 //An array saving the returned uniform locations

  //Now add a call to a specific element in the struct onto the line and check if it exists in shader
  //Check pos ,pnt
  uniLocs[0] = CheckUniformArrProp("spt_lights", shader_arr_index, "pos");
  //Check amb ,spt
  uniLocs[1] = CheckUniformArrProp("spt_lights", shader_arr_index, "amb");
  //Check dif ,spt
  uniLocs[2] = CheckUniformArrProp("spt_lights", shader_arr_index, "dif");
  //Check .spe ,spt
  uniLocs[3] = CheckUniformArrProp("spt_lights", shader_arr_index, "spe");


  //MATRIX

  //eIA is true, if any of the statements it is AND:ed with is false it will
  //turn false and this all remaining statements also become false
  for (int i = 0; i < 4; i++) {
    everythingIsAlright = everythingIsAlright && (uniLocs[i] != (GLint)-1);
  }

  //If nothing has gone wrong the uniform locations are saved
  if(everythingIsAlright) {
    this->m_dir_lights.s_uni_pos.push_back(uniLocs[0]);
    this->m_dir_lights.s_uni_amb.push_back(uniLocs[1]);
    this->m_dir_lights.s_uni_dif.push_back(uniLocs[2]);
    this->m_dir_lights.s_uni_spe.push_back(uniLocs[3]);

    //MATRIX
  }
}

void UploadPntLights(std::vector<PntLight> in_vec;) {
  for (int i = 0; i < in_vec.size(), i++) {

  }

}

void UploadDirLights(std::string arr_name, std::vector<DirLight> in_vec;) {
 //
}

void UploadDirLights(std::string arr_name, std::vector<DirLight> in_vec;) {
 //
}
