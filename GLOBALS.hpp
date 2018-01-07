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
const GLfloat PI = 3.1415926535f;
const int QUADTREE_ROOT_WIDTH = 64;
const int QUADTREE_MIN_WIDTH = 32;

// void PrintMat4(glm::mat4 matrix) {
//   float *mvp = (float*)glm::value_ptr(matrix);
//   std::cout << "this is the frustum matrix: " << '\n';
//   std::cout << mvp[0] << ", " << mvp[1] << ", " << mvp[2] << ", " << mvp[3] << '\n';
//   std::cout << mvp[4] << ", " << mvp[5] << ", " << mvp[6] << ", " << mvp[7] << '\n';
//   std::cout << mvp[8] << ", " << mvp[9] << ", " << mvp[10] << ", " << mvp[11] << '\n';
//   std::cout << mvp[12] << ", " << mvp[13] << ", " << mvp[14] << ", " << mvp[15] << '\n';
// }

#define COLOR_WHITE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_CYAN glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)


#endif
