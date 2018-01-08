#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "PackageStructs.hpp"

class QuadTree {
public:
<<<<<<< HEAD
  QuadTree(GLuint width);
=======
  QuadTree(GLuint width, GLuint minWidth);
>>>>>>> feature/terrain
  void GenerateTree(Node* parentNode, Node* nodePtr, GLuint xPos, GLuint zPos, GLuint width);
  void ReleaseNode(Node* currentNode);
  bool IsPointInNode(glm::vec3 point, Node* nodePtr);
  void InsertModelInTree(ModelData* modeldata);
  void FillModelPack(Node* nodePtr);
<<<<<<< HEAD
  ~QuadTree();
  std::vector<ModelData*> GetModelPack();
  void ClearModelPack();
  Node* GetRootNode();
private:
  const GLuint MIN_QUAD_SIZE = 16;
  GLuint m_nodeCounter;
  Node* m_rootNode;
=======
  std::vector<ModelData*> GetModelPack();
  void ClearModelPack();
  Node* GetRootNode();
  ~QuadTree();
private:
  Node* m_rootNode;
  GLuint MIN_QUAD_SIZE;
  GLuint m_nodeCounter;
>>>>>>> feature/terrain
  std::vector<ModelData*> m_modelPack;
};
