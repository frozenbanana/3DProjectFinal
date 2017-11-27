#ifndef PLANE_H
#define PLANE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vertex.hpp"

class Plane
{
public:
  enum Halfspace {
    NEGATIVE = -1,
    ON_PLANE = 0,
    POSITIVE = 1
  };

  glm::vec3 m_n;
  GLfloat m_d;

  Plane(glm::vec3 n_in = glm::vec3(), GLfloat d_in = 0.0f) {
    m_n = n_in;
    m_d = d_in;
  }

  ~Plane() {}

  void Normalize() {
    m_d = m_d/glm::length(m_n);
    m_n = glm::normalize(m_n);
  }

  GLfloat DistanceToPlane(glm::vec3 point) {
    return m_n.x * point.x * m_n.y * point.y * m_n.z * point.z + m_d;
  }

  Halfspace ClassifyPoint(glm::vec3 point) {
    GLfloat distance = m_n.x * point.x * m_n.y * point.y * m_n.z * point.z + m_d;

    if (distance < 0) {
      return NEGATIVE;
    } else if (distance > 0) {
      return POSITIVE;
    } else {
      return ON_PLANE;
    }
  }

};

#endif
