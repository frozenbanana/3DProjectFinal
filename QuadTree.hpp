#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "PackageStructs.hpp"

class QuadTree {
public:
  QuadTree(GLuint width);
  void GenerateTree(Node* parentNode, Node* nodePtr, GLuint xPos, GLuint zPos, GLuint width);
  void ReleaseNode(Node* currentNode);
  bool IsPointInNode(glm::vec3 point, Node* nodePtr);
  void InsertModelInTree(ModelData* modeldata);
  void FillModelPack(Node* nodePtr);
  ~QuadTree();
  std::vector<ModelData*> GetModelPack();
  void ClearModelPack();
  Node* GetRootNode();
private:
  const GLuint MIN_QUAD_SIZE = 16;
  GLuint m_nodeCounter;
  Node* m_rootNode;
  std::vector<ModelData*> m_modelPack;
};
