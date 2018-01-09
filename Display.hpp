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

  GLuint m_quadVAO;
  GLuint m_quadVBO;

  GBuffer m_gBuffer;

  glm::vec3 m_camPos;
  glm::mat4 m_view;
  glm::mat4 m_pers;

  Shader* m_geoShaderPtr;
  Shader* m_lgtShaderPtr;

  Terrain* m_terrain;
  void CreateQuad();
  void RenderQuad();
  void FixLightUniforms(Shader* shader_ptr, std::string pnt_str, std::string dir_str, std::string spt_str, int n_pnt, int n_dir, int n_spt);
  void RenderMesh(ModelData* modelData);
  void RenderMeshDR(ModelData* modelData);
  void UploadLightPack(Shader* shader_ptr, LightPack& lPack);
  void FixTextureUniforms(Shader* shader_ptr, std::string type_str, int n_tex);
  //void UploadTexture(Shader* shader_ptr, GLuint tex_id, int index);
public:
  Camera* m_camPtr, *m_camPtr2;
  bool m_camSwap;
  Shader* m_shaderPtr;
  GLfloat m_deltaTime;

  Display(int width, int height, const std::string& title, Camera* camPtr, Terrain* terrainPtr = nullptr);
  void SetTerrain(Terrain* terrainPtr);
  // SHADERS
  void SetShader(Shader* shaderPtr);
  void SetDRShaders(Shader* geoS, Shader* lgtS);

  // DRAWING
  void Draw(std::vector<ModelData*> modelPack, LightPack& lPack);
  void Draw(ModelData& modelData, LightPack& lPack);

  // Both GEO and LIGHT pass in same
  void DrawDR(std::vector<ModelData*> modelPack, LightPack& lPack);
  void DrawDR(ModelData& modelData, LightPack& lPack);

  // UPDATE
  void Update();
  void UpdateDR();

  bool IsClosed();
  void ToggleCamera();
  void SetExtraCamera(Camera* camPtr);
  void Clear(float r, float g, float b, float a);
  ~Display();
};

#endif
