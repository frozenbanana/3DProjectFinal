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
  bool s_insideFrustum;                               // is the model inside frustum
  GLenum s_mode;                                      // Which mode to render
  std::vector<GLuint> s_VAOs;                         // list of vaos for model
  std::vector< std::vector<GLuint> > s_meshIndices;   // list of indices for each mesh in model
  std::vector<std::vector<glm::vec3> > s_meshPos;     // list of position for every mesh
  std::vector< std::vector<Texture> > s_meshTextures; // list of textures for every mesh
  glm::vec3 s_localizePos[9];                         // extreme points of model. Used in frustum culling
  glm::mat4 s_modelMat;                               // model matrix for the model
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
