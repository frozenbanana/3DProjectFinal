#include "Light.hpp"

//Private


//Public
Light::Light(glm::vec4 col_amb, glm::vec4 col_dif, glm::vec4 col_spe) {
  this->m_col_amb = col_amb;
  this->m_col_dif = col_dif;
  this->m_col_spe = col_spe;
}

Light::~Light() {

}

glm::vec4 Light::getAmb() {
  return this->m_col_amb;
}
glm::vec4 Light::getDif() {
  return this->m_col_dif;
}
glm::vec4 Light::getSpe() {
  return this->m_col_spe;
}
