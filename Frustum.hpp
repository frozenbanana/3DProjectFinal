#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "Plane.hpp"
#include "Mesh.hpp"

class Frustum
{
private:
  enum PlanesPos{
    NEAR,
    FAR,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NUM_PLANES
  };

  Plane m_planes[NUM_PLANES];
public:
  Frustum(glm::mat4 matrix) {
    SetFrustum(matrix);
  }

 void SetFrustum(glm::mat4 matrix) {
    // Near clipping plane
    m_planes[NEAR].m_n.x = matrix[0][2];
    m_planes[NEAR].m_n.y = matrix[1][2];
    m_planes[NEAR].m_n.z = matrix[2][2];
    m_planes[NEAR].m_d = matrix[3][2];
    // Far clipping plane
    m_planes[FAR].m_n.x = matrix[0][3] - matrix[0][2];
    m_planes[FAR].m_n.y = matrix[1][3] - matrix[1][2];
    m_planes[FAR].m_n.z = matrix[2][3] - matrix[2][2];
    m_planes[FAR].m_d = matrix[3][3] - matrix[3][2];

    // Left clipping plane
    m_planes[LEFT].m_n.x = matrix[0][3] + matrix[0][0];
    m_planes[LEFT].m_n.y = matrix[1][3] + matrix[1][0];
    m_planes[LEFT].m_n.z = matrix[2][3] + matrix[2][0];
    m_planes[LEFT].m_d = matrix[3][3] + matrix[3][0];

    // Right clipping plane
    m_planes[RIGHT].m_n.x = matrix[0][3] - matrix[0][0];
    m_planes[RIGHT].m_n.y = matrix[1][3] - matrix[1][0];
    m_planes[RIGHT].m_n.z = matrix[2][3] - matrix[2][0];
    m_planes[RIGHT].m_d = matrix[3][3] - matrix[3][0];

    // Up clipping plane
    m_planes[UP].m_n.x = matrix[0][3] - matrix[0][1];
    m_planes[UP].m_n.y = matrix[1][3] - matrix[1][1];
    m_planes[UP].m_n.z = matrix[2][3] - matrix[2][1];
    m_planes[UP].m_d = matrix[3][3] - matrix[3][1];

    // Down clipping plane
    m_planes[DOWN].m_n.x = matrix[0][3] + matrix[0][1];
    m_planes[DOWN].m_n.y = matrix[1][3] + matrix[1][1];
    m_planes[DOWN].m_n.z = matrix[2][3] + matrix[2][1];
    m_planes[DOWN].m_d = matrix[3][3] + matrix[3][1];

    // Normalize the planes
    m_planes[NEAR].Normalize();
    m_planes[FAR].Normalize();
    m_planes[LEFT].Normalize();
    m_planes[RIGHT].Normalize();
    m_planes[UP].Normalize();
    m_planes[DOWN].Normalize();
  }

  ~Frustum() {}

  GLboolean InsideFrustrum(glm::vec3 p) {
    for (GLuint i = 0; i < NUM_PLANES; i++) {
      if(m_planes[i].ClassifyPoint(p) == Plane::NEGATIVE) {
        return false;
      }
    }

    return true;
  }

};
#endif


