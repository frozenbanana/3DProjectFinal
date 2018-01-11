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
  // bool s_hasNormalMap = false;
  GLenum s_mode;
  std::vector<GLuint> s_VAOs;
  std::vector< std::vector<GLuint> > s_meshIndices;
  std::vector< std::vector<glm::vec3> > s_meshPos;
  std::vector< std::vector<Texture> > s_meshTextures;
  Texture s_normalMap;
  glm::mat4 s_modelMat;
};

/*ModelPack*/
struct ModelPack {
  std::vector<ModelData> s_models;
};

//For Lights
struct LightPack {
  std::vector<PntLight> s_pnt_lights;
  std::vector<DirLight> s_dir_lights;
  std::vector<SptLight> s_spt_lights;
};

// For QuadTree
enum NodeIndex {
  TOP_LEFT = 0,
  TOP_RIGHT = 1,
  BOTTOM_LEFT = 2,
  BOTTOM_RIGHT = 3,

  NUM_OF_NODES = 4
};

struct Node {
  GLuint s_id;
  GLuint s_x, s_z;
  GLuint s_width;
  std::vector<ModelData*> s_models;
  Node* s_children[NUM_OF_NODES];
  Node* s_parent;
  bool s_isLeaf;
  bool s_insideFrustum;
};

#endif
