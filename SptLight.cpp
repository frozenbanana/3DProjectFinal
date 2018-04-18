#include "SptLight.hpp"

#include <glm/gtc/matrix_transform.hpp>

//Public
SptLight::SptLight(
  glm::vec3 pos,
  glm::vec3 poi,
  float n_plane,
  float f_plane,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) : Light(col_amb, col_dif, col_spe) {
    this->m_pos = pos;

    glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, n_plane, f_plane);
    glm::mat4 light_view = glm::lookAt(pos, poi, glm::vec3(0.0f, 1.0f, 0.0f));

    this->m_light_mat = light_projection * light_view;

}

SptLight::~SptLight(){

}

glm::vec3 SptLight::getPos() {
  return this->m_pos;
}

glm::mat4 SptLight::getLightMat() {
  return this->m_light_mat;
}
