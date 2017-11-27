#ifndef SPTLIGHT_H
#define SPTLIGHT_H

#include "Light.hpp"

class SptLight : public Light
{
private:
  glm::vec3 m_pos;

  glm::mat4 m_light_mat;

  //Save light map in here

public:
  SptLight(
    glm::vec3 pos,
    glm::vec3 poi,
    float n_plane,
    float f_plane,
    glm::vec4 col_amb,
    glm::vec4 col_dif,
    glm::vec4 col_spe
  );
  ~SptLight();

  glm::vec3 getPos();
  glm::mat4 getLightMat();
};

#endif
