#ifndef PNTLIGHT_H
#define PNTLIGHT_H

#include "Light.hpp"

class PntLight : public Light
{
private:
  glm::vec3 m_pos;

public:
  PntLight(
    glm::vec3 pos,
    glm::vec4 col_amb,
    glm::vec4 col_dif,
    glm::vec4 col_spe);
  ~PntLight();

  glm::vec3 getPos();
};

#endif
