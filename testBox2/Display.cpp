#include "Display.hpp"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title) {
	// start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    std::cout << "ERROR: could not start GLFW3" << std::endl;
    return;
  }

  m_window = glfwCreateWindow(640, 480, title.c_str(), NULL, NULL);
  if (!m_window) {
    std::cout << "ERROR: could not open window with GLFW3\n" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "Version: " << version << std::endl;

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
}


void Display::Update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    m_isClosed = true;
  }
}

void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed() {
	return this->m_isClosed;
}

Display::~Display() {
  glfwTerminate();
}
