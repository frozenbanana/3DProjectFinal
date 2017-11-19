#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Display
{
private:
  GLFWwindow* m_window;
  bool m_isClosed;
public:
  Display(int width, int height, const std::string& title);
  void Update();
  bool IsClosed();
  void Clear(float r, float g, float b, float a);
  ~Display();
};

#endif
