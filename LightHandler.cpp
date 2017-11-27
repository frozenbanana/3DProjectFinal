#include "LightHandler.hpp"

LightHandler::LightHandler() {
  //Hi I'm empty
}

LightHandler::~LightHandler() {

}

void LightHandler::AddPntLight(glm::vec3 pos,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) {
    this->m_light_pack.s_pnt_lights.push_back(PntLight(pos, col_amb, col_dif, col_spe));
}

void LightHandler::AddDirLight(glm::vec3 dir,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) {
    this->m_light_pack.s_dir_lights.push_back(DirLight(dir, col_amb, col_dif, col_spe));
}

void LightHandler::AddSptLight(glm::vec3 pos,
  glm::vec3 poi,
  float n_plane,
  float f_plane,
  glm::vec4 col_amb,
  glm::vec4 col_dif,
  glm::vec4 col_spe) {
    this->m_light_pack.s_spt_lights.push_back(SptLight(pos, poi, n_plane, f_plane, col_amb, col_dif, col_spe));
}

LightPack LightHandler::GetLightPack() {
  return this->m_light_pack;
}
