#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Transform.hpp"
#include "Mesh.hpp"
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

class Model : public Transform
{
private:
  std::vector<Mesh> m_meshes;
  std::string m_directory;
  Transform m_transform;
  std::vector<Texture> m_textures_loaded;
  std::vector<GLuint> m_vaos;
  std::vector<GLuint> m_vbos;
  std::vector<GLuint> m_ebos;
  void LoadModel(std::string path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
  std::vector<GLuint> GetVAOs();
  std::vector<GLuint> GetVBOs();
  std::vector<GLuint> GetEBOs();
  std::vector<std::vector<GLuint> > GetModelMeshesIndices();
  std::vector<std::vector<glm::vec3> > GetModelMeshesPos();
  ModelData m_modelData;
public:
  Model(std::string path);
  std::vector<Mesh> GetModelMeshes();
  ModelData& GetModelData();
  ~Model();
};

#endif
