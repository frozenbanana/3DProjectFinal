#ifndef PACKAGESTRUCTS_H
#define PACKAGESTRUCTS_H
#include <vector>
#include <GL/glew.h>

#include "Mesh.hpp"
#include "PntLight.hpp"
#include "DirLight.hpp"
#include "SptLight.hpp"

//For Models
struct ModelData {
  bool s_insideFrustum;
  GLenum s_mode;
  std::vector<GLuint> s_VAOs;
  std::vector< std::vector<GLuint> > s_meshIndices;
  std::vector<std::vector<glm::vec3> > s_meshPos;
  std::vector< std::vector<Texture> > s_meshTextures;
  glm::mat4 s_modelMat;
};

/*ModelPack*/
struct ModelPack {
  std::vector<ModelData> s_Models;
};

//For Lights
struct LightPack {
  std::vector<PntLight> s_pnt_lights;
  std::vector<DirLight> s_dir_lights;
  std::vector<SptLight> s_spt_lights;
};

#endif
