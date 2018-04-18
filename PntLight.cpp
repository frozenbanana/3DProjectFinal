#include "PntLight.hpp"

//Public
PntLight::PntLight(
  glm::vec3 pos,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe,
  float c,
  float l,
  float q) : Light(col_amb, col_dif, col_spe) {
    this->m_pos = pos;
    this->constant = c;
    this->linear = l;
    this->quadratic = q;
}

PntLight::~PntLight(){

}

glm::vec3 PntLight::getPos() {
  return this->m_pos;
}

float PntLight::getConstant() {
  return this->constant;
}

float PntLight::getLinear() {
  return this->linear;
}

float PntLight::getQuadratic() {
  return this->quadratic;
}
