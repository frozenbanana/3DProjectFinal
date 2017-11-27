#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <GL/glew.h> 
#include <vector>
#include <glm/glm.hpp>
#include "Mesh.hpp"

const int WINDOW_HEIGHT = 640;
const int WINDOW_WIDTH = 480;
const GLfloat PI = 3.1415926535f;

struct ModelData {
  std::vector<GLuint> s_VAOs;
  std::vector<std::vector<GLuint> > s_meshIndices;
  std::vector<std::vector<glm::vec3> > s_meshPos;
  glm::mat4 s_modelMat;
};

#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec3(0.0f, 0.0f, 1.0f)

#endif
