#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "Camera.hpp"
#include "Shader.hpp"
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

void MouseCallback(GLFWwindow* winPtr, double xPos, double yPos);
void KeyCallback(GLFWwindow* winPtr, int key, int scan, int act, int mode);
extern bool g_firstMouse;
extern GLfloat g_lastX;
extern GLfloat g_lastY;
extern GLfloat g_xoffset;
extern GLfloat g_yoffset;
extern GLfloat g_yaw;
extern GLfloat g_pitch;
//Array in which the data for key presses is stored (used in key_callback())
extern bool g_key_data[1024];

class Display
{
private:
  GLFWwindow* m_window;
  bool m_isClosed;
  GLfloat m_lastFrame;

  void FixLightUniforms(std::string pnt_str, std::string dir_str, std::string spt_str, int n_pnt, int n_dir, int n_spt);
  void UploadLightPack(LightPack& lPack);
public:
  Display(int width, int height, const std::string& title, Camera* camPtr);
  Camera* m_camPtr;
  Shader* m_shaderPtr;
  GLfloat m_deltaTime;
  void SetShader(Shader* shaderPtr);
  void Draw(ModelData& modelData, LightPack& lPack);
  void Update();
  bool IsClosed();
  void Clear(float r, float g, float b, float a);
  ~Display();
};

#endif
