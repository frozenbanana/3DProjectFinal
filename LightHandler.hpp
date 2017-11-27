#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

#include <vector>

#include "Light.hpp"
#include "PackageStructs.hpp"

class LightHandler
{
private:
  LightPack m_light_pack;
public:
  LightHandler();
  ~LightHandler();

  void AddPntLight(glm::vec3 pos, glm::vec4 col_amb, glm::vec4 col_dif, glm::vec4 col_spe);
  void AddDirLight(glm::vec3 dir, glm::vec4 col_amb, glm::vec4 col_dif, glm::vec4 col_spe);
  void AddSptLight(glm::vec3 pos, glm::vec3 poi, float n_plane, float f_plane, glm::vec4 col_amb, glm::vec4 col_dif, glm::vec4 col_spe);

  LightPack GetLightPack();
};


#endif
