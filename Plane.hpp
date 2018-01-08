#ifndef PLANE_H
#define PLANE_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vertex.hpp"
#include <iostream>

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

  Plane(glm::vec4 plane_in = glm::vec4()) {
    SetPlane(plane_in);
  }
  ~Plane() {}

  void SetPlane(glm::vec4 plane_in) {
    m_n.x = plane_in.x;
    m_n.y = plane_in.y;
    m_n.z = plane_in.z;
    m_d = plane_in.w;
  }

  void Set(float x, float y, float z, float d) {
    m_n.x = x;
    m_n.y = y;
    m_n.z = z;
    m_d = d;
  }

  void Normalize() {
    float length = sqrt(m_n.x * m_n.x + m_n.y * m_n.y + m_n.z * m_n.z);
	  m_n.x /= length;
	  m_n.y /= length;
	  m_n.z /= length;
	  m_d /= length;
  }

  // Make sure planes are normalized! OK
  GLfloat DistanceToPlane(glm::vec3 point) {
    return m_n.x * point.x + m_n.y * point.y + m_n.z * point.z + m_d;
  }

  Halfspace ClassifyPoint(glm::vec3 point) {
    GLfloat dot = m_n.x * point.x + m_n.y * point.y + m_n.z * point.z + m_d;
    if (dot < 0) {
      return NEGATIVE;
    } else if (dot > 0) {
      return POSITIVE;
    } else {
      return ON_PLANE;
    }
  }

  std::string ToString() {
    std::stringstream ss;
    ss << m_n.x << ", " << m_n.y << ", " << m_n.z << ", " << m_d << std::endl;
    return ss.str();
  }
};

#endif
