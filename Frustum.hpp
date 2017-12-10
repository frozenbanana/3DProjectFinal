#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "Plane.hpp"
#include "Mesh.hpp"
#include "PackageStructs.hpp"
#include <glm/gtc/matrix_access.hpp>

const GLuint NUM_PLANES = 6;
class Frustum
{
private:
  Plane m_planes[NUM_PLANES];
public:
  Frustum(glm::mat4& matrix) {
    SetFrustum(matrix);
  }

  void SetFrustum(glm::mat4& matrix) {

    glm::vec4 rowX = glm::column(matrix, 0);
    glm::vec4 rowY = glm::column(matrix, 1);
    glm::vec4 rowZ = glm::column(matrix, 2);
    glm::vec4 rowW = glm::column(matrix, 3);
    m_planes[0].SetPlane(rowW + rowX);
    m_planes[1].SetPlane(rowW - rowX);
    m_planes[2].SetPlane(rowW + rowY);
    m_planes[3].SetPlane(rowW - rowY);
    m_planes[4].SetPlane(rowW + rowZ);
    m_planes[5].SetPlane(rowW - rowZ);

    m_planes[0].Normalize();
    m_planes[1].Normalize();
    m_planes[2].Normalize();
    m_planes[3].Normalize();
    m_planes[4].Normalize();
    m_planes[5].Normalize();
  }

  ~Frustum() {}

  GLboolean InsideFrustrum(glm::vec3 point) {
    for (GLuint i = 0; i < NUM_PLANES; i++) {
      if(m_planes[i].ClassifyPoint(point) == Plane::NEGATIVE) {
        return false;
      }
    }
    return true;
  }


  void CullMeshes(ModelData* modelData) {
    GLuint meshCount = modelData->s_meshPos.size();
    modelData->s_insideFrustum = false;
    for(GLuint i = 0; i < meshCount && !modelData->s_insideFrustum; i++) {
      // Check every point in current mesh
      for(GLuint j = 0; j < modelData->s_meshPos[i].size() && !modelData->s_insideFrustum; j++) {
	// Keep looping until one point is inside frustum 
	modelData->s_insideFrustum = InsideFrustrum(modelData->s_meshPos[i][j]);
      }
    } 
  }

};

#endif
