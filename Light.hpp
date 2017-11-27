#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
protected:
  glm::vec4 m_col_amb;
  glm::vec4 m_col_dif;
  glm::vec4 m_col_spe;

public:
  Light(glm::vec4 col_amb, glm::vec4 col_dif, glm::vec4 col_spe);
  ~Light();

  glm::vec4 getAmb();
  glm::vec4 getDif();
  glm::vec4 getSpe();
};

#endif  //LIGHT_H
