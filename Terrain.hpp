#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <iostream>
#include <vector>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include "Mesh.hpp"
#include "PackageStructs.hpp"
#include "texturefunctions.hpp"

struct BMPData {
  unsigned char header[52];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;
  unsigned char* data;
  bool good;
};

class Terrain {
public:
  Terrain(std::string filePath, GLuint maxHeight = 10, std::string diffTexPath = "unknown", std::string specTexPath = "unknown", std::string normTexPath = "unknown");
  ModelData& GetModelData();
  GLfloat GetHeight(GLfloat xPos, GLfloat zPos);
  ~Terrain();
private:
  BMPData m_BMPData;
  ModelData m_modelData;
  Mesh* m_terrainMeshPtr;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  std::vector<Texture> m_textures;

  GLuint m_width, m_height;
  GLuint MAX_HEIGHT;

  bool computedNormals;
  bool computedIndices;

  void PrepModelData();
  void LoadBMPData(std::string fileName);
  void SetMeshData(BMPData BMPData);
  void SetHeight(unsigned zPos, unsigned xPos, float height);
  void ComputePos();
  void ComputeNormals();
  void ComputeTexCoords();
  void ComputeTangents();
  void TextureFromFile(const char *path, std::string typeName, Texture& texture);
  // helper function
  void ComputeTangentBasis(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2,
		const glm::vec2& UV0, const glm::vec2& UV1, const glm::vec2& UV2,
		glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent);

  void ComputeIndices();

};

#endif
