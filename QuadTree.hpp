// TODO Debug
#include "glm/glm.hpp"
#include "PackageStructs.hpp"

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
  GLuint s_VAOs;
  bool s_isLeaf;
  Node* s_children[NUM_OF_NODES];
  Node* s_parent;
};

class QuadTree {
public:
  QuadTree(GLuint width) {
    std::cout << "---Let's start this quad tree!" << '\n';
    m_nodeCounter = 0;
    std::cout << "------m_rootNode:" << m_rootNode << '\n';
    GenerateTree(nullptr, m_rootNode, 0, 0, width);
    std::cout << "---this quad tree is finished" << '\n';
    std::cout << "rootNode has" << m_rootNode->s_id << '\n';
  }

  void GenerateTree(Node* parentNode, Node* nodePtr, GLuint xPos, GLuint zPos, GLuint width) {
    // Init current node
    if (nodePtr->s_id == m_rootNode->s_id) {
      std::cout << "they have same id" << '\n';
    }
    // nodePtr = new Node();
    nodePtr->s_id = m_nodeCounter++;
    nodePtr->s_parent = parentNode;
    nodePtr->s_x = xPos;
    nodePtr->s_z = zPos;
    nodePtr->s_width = width;
    nodePtr->s_isLeaf = false;
    std::cout << "New node has been created: " << nodePtr->s_id << '\n';

    // check if it has reached lowest resolution
    if (width <= MIN_QUAD_SIZE) {
      std::cout << "width is lower or equal then " << MIN_QUAD_SIZE << '\n';
      nodePtr->s_isLeaf = true;

      for (size_t i = 0; i < NUM_OF_NODES; i++) {
        nodePtr->s_children[i] = nullptr;
      }
      return;
    } else {
      GLuint widthChild = nodePtr->s_width / 2;
      std::cout << "I am producing 4 new nodes with "<< widthChild << " x " << " and ID: " << nodePtr->s_id << '\n';
      GenerateTree(nodePtr, nodePtr->s_children[TOP_LEFT],     xPos,              zPos + widthChild, widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[TOP_RIGHT],    xPos + widthChild, zPos + widthChild, widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[BOTTOM_LEFT],  xPos,              zPos,              widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[BOTTOM_RIGHT], xPos + widthChild, zPos,              widthChild);
    }
  }

  void ReleaseNode(Node* currentNode) {
    std::cout << "Entering ReleaseNode with: " << currentNode->s_id << '\n';
    if (!currentNode->s_isLeaf) {
      for (size_t i = 0; i < NUM_OF_NODES; i++) {
          ReleaseNode(currentNode->s_children[i]);
        }
    }
    else {
      for (size_t i = 0; i < NUM_OF_NODES; i++) {
        if (currentNode->s_children[i]->s_isLeaf) {
          std::cout << "Current Node to be released is: " << currentNode->s_id << '\n';
          delete currentNode->s_children[i];
          currentNode->s_children[i] = 0;
        }
      }
    }
  }

  ~QuadTree() {
  }

  Node* m_rootNode;
private:
  const GLuint MIN_QUAD_SIZE = 32;
  GLuint m_nodeCounter;
  void InsertModelInTree(ModelData modeldata);
  //ModelPack SendModels();
};
