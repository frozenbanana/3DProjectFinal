#include <iostream>
#include <vector>
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
<<<<<<< HEAD
  GLuint s_VAOs;
  bool s_isLeaf;
  Node* s_children[NUM_OF_NODES];
  Node* s_parent;
=======
  std::vector<GLuint> s_VAOs;
  Node* s_children[NUM_OF_NODES];
  Node* s_parent;
  bool s_isLeaf;
  bool s_insideFrustum;
>>>>>>> CullNode implemented but not tested
};

class QuadTree {
public:
  QuadTree(GLuint width) {
<<<<<<< HEAD
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
=======
    m_nodeCounter = 0;
    m_rootNode = new Node();
    GenerateTree(nullptr, m_rootNode, 0, 0, width);
  }

  //  Input a node*, init node and connect 4 new nodes until highest resolution is met
  void GenerateTree(Node* parentNode, Node* nodePtr, GLuint xPos, GLuint zPos, GLuint width) {
    // Init current node
>>>>>>> CullNode implemented but not tested
    nodePtr->s_id = m_nodeCounter++;
    nodePtr->s_parent = parentNode;
    nodePtr->s_x = xPos;
    nodePtr->s_z = zPos;
    nodePtr->s_width = width;
    nodePtr->s_isLeaf = false;
<<<<<<< HEAD
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
=======
    nodePtr->s_insideFrustum = true;

    std::cout << "New node, id: " << nodePtr->s_id;

    // check if it has reached highest resolution
    if (width <= MIN_QUAD_SIZE) { // width <= 32
      std::cout << "is a leaf!" << std::endl;
      nodePtr->s_isLeaf = true;

      // Make children nullptr
      for (size_t i = 0; i < NUM_OF_NODES; i++) {
        nodePtr->s_children[i] = nullptr;
      }
      // Then stop generating more nodes
      return;
    }
    else {
      std::cout << '\n';
      GLuint widthChild = nodePtr->s_width / 2;
      for (size_t i = 0; i < NUM_OF_NODES; i++) {
        nodePtr->s_children[i] = new Node();
      }

>>>>>>> CullNode implemented but not tested
      GenerateTree(nodePtr, nodePtr->s_children[TOP_LEFT],     xPos,              zPos + widthChild, widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[TOP_RIGHT],    xPos + widthChild, zPos + widthChild, widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[BOTTOM_LEFT],  xPos,              zPos,              widthChild);
      GenerateTree(nodePtr, nodePtr->s_children[BOTTOM_RIGHT], xPos + widthChild, zPos,              widthChild);
    }
  }

  void ReleaseNode(Node* currentNode) {
<<<<<<< HEAD
    std::cout << "Entering ReleaseNode with: " << currentNode->s_id << '\n';
=======
>>>>>>> CullNode implemented but not tested
    if (!currentNode->s_isLeaf) {
      for (size_t i = 0; i < NUM_OF_NODES; i++) {
          ReleaseNode(currentNode->s_children[i]);
        }
    }
    else {
<<<<<<< HEAD
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
=======
        delete currentNode;
    }
  }

  bool IsPointInNode(glm::vec3 point, Node* nodePtr) {
    glm::vec3 nodePos((float)nodePtr->s_x, 0, (float)nodePtr->s_z);
    float nodeWidth = (float)nodePtr->s_width;

    if (point.x <= (nodePos.x + nodeWidth) &&
        point.x >= nodePos.x &&
        point.z <= (nodePos.z + nodeWidth) &&
        point.z >= nodePos.z) {
            return true;
    }
    else {
    return false;
    }
  }

  void InsertModelInTree(ModelData modeldata) {
    int correctIndex = -1;
    // Assume modeldatas vertecies are in World space
    glm::vec3 modelPos = modeldata.s_meshPos[0][0];
    // compare model with node pos to determine best locataion for model
    Node* currentNodePtr = m_rootNode;
    bool activeNode = false;
    while (!currentNodePtr->s_isLeaf) {
      for (int i = 0; i < 4 && !activeNode; i++) {
        activeNode = IsPointInNode(modelPos, currentNodePtr->s_children[i]);
        correctIndex = i;
      }
      currentNodePtr = currentNodePtr->s_children[correctIndex];
    }
    currentNodePtr->s_VAOs = modeldata.s_VAOs;
  }

  // TODO cull quadtree with frustum
  ~QuadTree() {
    ReleaseNode(m_rootNode);
  }
private:
  Node* m_rootNode;
  const GLuint MIN_QUAD_SIZE = 32;
  GLuint m_nodeCounter;
>>>>>>> CullNode implemented but not tested
  //ModelPack SendModels();
};
