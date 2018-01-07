#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "Plane.hpp"
#include "Mesh.hpp"
#include "PackageStructs.hpp"
#include "GLOBALS.hpp"

class Frustum
{
private:
  enum {
    RIGHTP = 0,
    LEFTP = 1,
    NEARP = 2,
    FARP = 3,
    BOTTOMP = 4,
    TOPP = 5,

    NUM_PLANES = 6
  };

  Plane m_planes[NUM_PLANES];
public:
  Frustum(glm::mat4 matrix) {
    SetFrustum(matrix);
  }

  void SetFrustum(glm::mat4 matrix) {
    float *vp = (float*)glm::value_ptr(matrix);
    // std::cout << "this is the frustum matrix: " << '\n';
    // std::cout << mvp[0] << ", " << mvp[1] << ", " << mvp[2] << ", " << mvp[3] << '\n';
    // std::cout << mvp[4] << ", " << mvp[5] << ", " << mvp[6] << ", " << mvp[7] << '\n';
    // std::cout << mvp[8] << ", " << mvp[9] << ", " << mvp[10] << ", " << mvp[11] << '\n';
    // std::cout << mvp[12] << ", " << mvp[13] << ", " << mvp[14] << ", " << mvp[15] << '\n';
		m_planes[RIGHTP].Set(  vp[3] - vp[0], vp[7] - vp[4], vp[11] - vp[8],  vp[15] - vp[12] ); // Right clipping plane.
		m_planes[LEFTP].Set(   vp[3] + vp[0], vp[7] + vp[4], vp[11] + vp[8],  vp[15] + vp[12] ); // Left clipping plane.
		m_planes[NEARP].Set(   vp[3] + vp[2], vp[7] + vp[6], vp[11] + vp[10], vp[15] + vp[14] ); // Near clipping plane.
		m_planes[FARP].Set(    vp[3] - vp[2], vp[7] - vp[6], vp[11] - vp[10], vp[15] - vp[14] ); // Far clipping plane.
		m_planes[BOTTOMP].Set( vp[3] + vp[1], vp[7] + vp[5], vp[11] + vp[9],  vp[15] + vp[13] ); // Bottom clipping plane.
		m_planes[TOPP].Set(    vp[3] - vp[1], vp[7] - vp[5], vp[11] - vp[9],  vp[15] - vp[13] ); // Top clipping plane.

    // Normalize planes
    for (size_t i = 0; i < NUM_PLANES; i++) {
      m_planes[i].Normalize();
    }
  }

  ~Frustum() {}

  GLboolean InsideFrustrum(glm::vec3 point) {
     // for (GLuint i = 0; i < 1; i++) {    // Only need to check left, right, far and near plane
        // std::cout << " Distance to plane[" << i << "]: " << m_planes[i].DistanceToPlane(point);
       if (m_planes[0].ClassifyPoint(point) < 0.0f) {
         std::cout << "CULLED" << '\n';
         return false;
       }
    // }

    // std::cout << '\n';
    return true;
  }

  // Culls all node recursivly
  void CullNode(Node* nodePtr) {
    // Prepare node for checking
    nodePtr->s_insideFrustum = false;

    // convert nodePtr to floats to make corners
    float xPos = (float)nodePtr->s_x;
    float zPos = (float)nodePtr->s_z;
    float nodeWidth = (float)nodePtr->s_width;

    // Create corners from node pos to test for frustum
    glm::vec3 nodeCorners[4];
    nodeCorners[0] = glm::vec3(xPos,                  0, zPos + nodeWidth); // TOP_LEFT
    nodeCorners[1] = glm::vec3(xPos + nodeWidth,      0, zPos + nodeWidth); // TOP_RIGHT
    nodeCorners[2] = glm::vec3(xPos,                  0, zPos);             // BOTTOM_LEFT
    nodeCorners[3] = glm::vec3(xPos + nodeWidth,      0, zPos);             // BOTTOM_RIGHT

    // Cull node corners
    // for (size_t i = 0; i < 1 && !nodePtr->s_insideFrustum; i++) {
    	nodePtr->s_insideFrustum = InsideFrustrum(nodeCorners[0]);
    // }

    if (nodePtr->s_id == 3)
      std::cout << "Looking at the nodeID: " << nodePtr->s_id << ", inside? "<< nodePtr->s_insideFrustum << '\n';

    // If we are at bottom jump out
    if (nodePtr->s_isLeaf) {
      return;
    }

    // Keep doing it until leaf
    if (nodePtr->s_insideFrustum) {
      // CullNode(nodePtr->s_children[TOP_LEFT]);
      // CullNode(nodePtr->s_children[TOP_RIGHT]);
      CullNode(nodePtr->s_children[BOTTOM_LEFT]);
      // CullNode(nodePtr->s_children[BOTTOM_RIGHT]);
    }
  }

  void CullMeshes(ModelData* modelData) {
    GLuint meshCount = modelData->s_meshPos.size();
    glm::vec3 worldPos;
    modelData->s_insideFrustum = false;
    for(GLuint i = 0; i < meshCount && !modelData->s_insideFrustum; i++) {
      // Check every point in current mesh
      for(GLuint j = 0; j < modelData->s_meshPos[i].size() && !modelData->s_insideFrustum; j++) {
      	// Keep looping until one point is inside frustum
        worldPos = glm::vec3(modelData->s_modelMat * glm::vec4(modelData->s_meshPos[i][j], 1.0f));
      	modelData->s_insideFrustum = InsideFrustrum(worldPos);
      }
    }
  }

};

#endif
