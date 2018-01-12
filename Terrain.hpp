#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <iostream>
#include <vector>
#include "Mesh.hpp"
#include "PackageStructs.hpp"

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
  Terrain(std::string fileName, GLuint maxHeight = 10);
  ModelData& GetModelData();
  GLfloat GetHeight(GLfloat xPos, GLfloat zPos);
  ~Terrain();
private:
  BMPData m_BMPData;
  ModelData m_modelData;
  Mesh* m_terrainMeshPtr;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

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
  void ComputeIndices();
  void ComputeTangents();
};

#endif
