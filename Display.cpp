#include "Display.hpp"
#include <GL/glew.h>
#include <iostream>

bool g_firstMouse = true;
GLfloat g_lastX = 640 / 2.0;
GLfloat g_lastY = 480 / 2.0;
GLfloat g_xoffset = 640 / 2.0;
GLfloat g_yoffset = 480 / 2.0;
GLfloat g_yaw = -90.0f;
GLfloat g_pitch = 0.0f;
//Array in which the data for key presses is stored (used in key_callback())
bool g_key_data[1024];

Display::Display(int width, int height, const std::string& title, Camera* camPtr) {
  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    std::cout << "ERROR: could not start GLFW3" << std::endl;
    return;
  }
  // Set appropriate openGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window
  m_window = glfwCreateWindow(640, 480, title.c_str(), NULL, NULL);
  if (!m_window) {
    std::cout << "ERROR: could not open window with GLFW3\n" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);

  // Bind camera to display
  m_camPtr = camPtr;

  //Set the window to capture the mouse and hide it. It's like a cat but digital
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //Set callback functions
  glfwSetKeyCallback(m_window, KeyCallback);				//This sets the basic key_callback function 'key_callback' as default.
  glfwSetCursorPosCallback(m_window, MouseCallback);		//This sets the basic mouse_callback function 'mouse_callback' as default.

  // second argument could be a struct of relevant pointers to control in callbackfunctions down below
  glfwSetWindowUserPointer(m_window, this);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "Version: " << version << std::endl;

  m_deltaTime = 0.0f;
  m_lastFrame = 0.0f;
  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
}


void Display::Update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
  // Set frame time
  GLfloat currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

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

void Display::DoMovement() {
  // Camera controls
}

Display::~Display() {
  glfwTerminate();
}


void KeyCallback(GLFWwindow* winPtr, int key, int scan, int act, int mode) {
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) {
		glfwSetWindowShouldClose(winPtr, GL_TRUE);
	}

	// ! setting global variable
	//Store the data from when a key is pressed
	if (key >= 0 && key < 1024)
  {
      if (act == GLFW_PRESS) {
        g_key_data[key] = true;
      }
      else if (act == GLFW_RELEASE) {
        g_key_data[key] = false;
      }
  }
  Display* d = (Display*)glfwGetWindowUserPointer(winPtr);
	if (g_key_data[GLFW_KEY_W])
		d->m_camPtr->ProcessKeyboard(FORWARD, d->m_deltaTime);
	if (g_key_data[GLFW_KEY_S])
		d->m_camPtr->ProcessKeyboard(BACKWARD, d->m_deltaTime);
	if (g_key_data[GLFW_KEY_A])
		d->m_camPtr->ProcessKeyboard(LEFT, d->m_deltaTime);
	if (g_key_data[GLFW_KEY_D])
		d->m_camPtr->ProcessKeyboard(RIGHT, d->m_deltaTime);
	if (g_key_data[GLFW_KEY_SPACE])
    d->m_camPtr->ProcessKeyboard(UP, d->m_deltaTime);
}

void MouseCallback(GLFWwindow* winPtr, double xPos, double yPos) {
	if (g_firstMouse)
    {
      g_lastX = xPos;
      g_lastY = yPos;
      g_firstMouse = false;
    }

  g_xoffset = xPos - g_lastX;
  g_yoffset = g_lastY - yPos;

	g_lastX = xPos;
	g_lastY = yPos;

  Display* d = (Display*)glfwGetWindowUserPointer(winPtr);
  d->m_camPtr->ProcessMouseMovement(g_xoffset, g_yoffset);
}


