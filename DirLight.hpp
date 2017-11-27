#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "Light.hpp"

class DirLight : public Light
{
private:
  glm::vec3 m_dir;

public:
  DirLight(
    glm::vec3 dir,
    glm::vec4 col_amb,
    glm::vec4 col_dif,
    glm::vec4 col_spe);
  ~DirLight();

  glm::vec3 getDir();
};

#endif
