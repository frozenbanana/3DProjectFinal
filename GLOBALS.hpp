#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.hpp"

//Constants
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;
const int SHADOW_HEIGHT = 1024;
const int SHADOW_WIDTH = 1024;
const GLfloat PI = 3.1415926535f;
const int QUADTREE_ROOT_WIDTH = 512;
const int QUADTREE_MIN_WIDTH = 8;

#define COLOR_WHITE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLACK glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
#define COLOR_RED glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_CYAN glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)


#endif
