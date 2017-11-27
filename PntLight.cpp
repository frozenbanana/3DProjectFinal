#include "PntLight.hpp"

//Private



//Public
PntLight::PntLight(
  glm::vec3 pos,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) : Light(col_amb, col_dif, col_spe) {
    this->m_pos = pos;
}

PntLight::~PntLight(){

}

glm::vec3 PntLight::getPos() {
  return this->m_pos;
}
