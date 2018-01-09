#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "Camera.hpp"
#include "Shader.hpp"
#include "Terrain.hpp"
#include "GBuffer.hpp"
#include "PingPongBuffer.hpp"
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

  int m_width;
  int m_height;

  Camera* m_camPtr2;
  glm::vec3 m_camPos;
  bool m_camSwap;

  GLuint m_quadVAO;
  GLuint m_quadVBO;

  GBuffer m_gBuffer;
  PingPongBuffer m_ppBuffer;

  Shader* m_shaderPtr;
  Shader* m_geoShaderPtr;
  Shader* m_lgtShaderPtr;

  Terrain* m_terrain;
  Shader* m_comShaderPtr;

  glm::mat4 m_view;
  glm::mat4 m_pers;

  void CreateQuad();
  void RenderQuad();
  void FixLightUniforms(Shader* shader_ptr, std::string pnt_str, std::string dir_str, std::string spt_str, int n_pnt, int n_dir, int n_spt);
  void RenderMesh(ModelData* modelData);
  void RenderMeshDR(ModelData* modelData);
  void UploadLightPack(Shader* shader_ptr, LightPack& lPack);
  void FixTextureUniforms(Shader* shader_ptr, std::string type_str, int n_tex);
  //void UploadTexture(Shader* shader_ptr, GLuint tex_id, int index);
public:
  Camera* m_camPtr;
  GLfloat m_deltaTime;

  Display(int width, int height, const std::string& title, Camera* camPtr, Terrain* terrainPtr = nullptr);
  ~Display();

  bool IsClosed();
  void SetExtraCamera(Camera* camPtr);
  void SetTerrain(Terrain* terrainPtr);
  void ToggleCamera();
  void Clear(float r, float g, float b, float a);

  // SIMPLE
  void SetShader(Shader* shaderPtr);
  void Update();
  void Draw(ModelData& modelData, LightPack& lPack);
  void Draw(std::vector<ModelData*> modelPack, LightPack& lPack);

  // ADVANCED
  void SetDRShaders(Shader* geoS, Shader* lgtS);
  void SetComputeShader(Shader* comS, Shader* tarS);
  void UpdateDR();
  void DrawDR(ModelData& modelData, LightPack& lPack);
  void DrawDR(std::vector<ModelData*> modelPack, LightPack& lPack);

};

#endif
