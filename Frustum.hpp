#ifndef FRUSTUM_H
#define FRUSTUM_H
#include "Plane.hpp"
#include "Mesh.hpp"
#include "PackageStructs.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

class Frustum
{
private:
  enum {
    RIGHTP,
    LEFTP,
    BOTTOMP,
    TOPP,
    FARP,
    NEARP,

    NUM_PLANES
  };

  Plane m_planes[NUM_PLANES];
public:
  Frustum(glm::mat4 matrix) {
    SetFrustum(matrix);
  }

  void SetFrustum(glm::mat4 matrix) {
    float *mvp = (float*)glm::value_ptr(matrix);
    m_planes[LEFTP].Set(   mvp[3] - mvp[0], mvp[7] - mvp[4], mvp[11] - mvp[8],  mvp[15] - mvp[12] );
    m_planes[RIGHTP].Set(  mvp[3] + mvp[0], mvp[7] + mvp[4], mvp[11] + mvp[8],  mvp[15] + mvp[12] );
    m_planes[BOTTOMP].Set( mvp[3] + mvp[1], mvp[7] + mvp[5], mvp[11] + mvp[9],  mvp[15] + mvp[13] );
    m_planes[TOPP].Set(    mvp[3] - mvp[1], mvp[7] - mvp[5], mvp[11] - mvp[9],  mvp[15] - mvp[13] );
    m_planes[FARP].Set(    mvp[3] - mvp[2], mvp[7] - mvp[6], mvp[11] - mvp[10], mvp[15] - mvp[14] );
    m_planes[NEARP].Set(   mvp[3] + mvp[2], mvp[7] + mvp[6], mvp[11] + mvp[10], mvp[15] + mvp[14] );
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

  void GetModelData() {
  }
    
};

#endif
