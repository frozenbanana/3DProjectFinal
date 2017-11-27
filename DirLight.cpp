#include "DirLight.hpp"

//Private



//Public
DirLight::DirLight(
  glm::vec3 dir,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) : Light(col_amb, col_dif, col_spe) {
    this->m_dir = dir;
}

DirLight::~DirLight(){

}

glm::vec3 DirLight::getDir(){
  return this->m_dir;
}
