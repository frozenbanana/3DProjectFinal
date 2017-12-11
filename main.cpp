#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>

//View Domain
#include "Display.hpp"
#include "Shader.hpp"

//Model Domain
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Frustum.hpp"
#include "LightHandler.hpp"

//Trickster Domain
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

int main() {
  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 0.0f, -5.0f));
  Display display(WINDOW_HEIGHT, WINDOW_WIDTH, "Test terrain class", &camera);
  Shader shader(vertex_shader, fragment_shader);
  display.SetShader(&shader);   //Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP MODELS
  Model model1("res/models/nano/nanosuit.obj");
  Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  model2.SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
  model2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

  // PACKAGE MODEL DATA 
  ModelData modelData1 = model1.GetModelData();
  ModelData modelData2 = model2.GetModelData();

  Frustum frustum(camera.GetViewPersMatrixRef());
  
  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(glm::vec3(0.0f, 10.0f, 0.0f), COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

  //PACKAGE LIGHT DATA TO DISPLAY (STATIC)
  LightPack lPack = lightHandler.GetLightPack();

  // DRAW LOOP
  while(!display.IsClosed()) {
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
    frustum.SetFrustum(camera.GetViewPersMatrixRef());
    frustum.CullMeshes(&modelData1); // modified
    frustum.CullMeshes(&modelData2); // modified
    display.Draw(modelData1, lPack);
    display.Draw(modelData2, lPack);
    display.Update();
  }

  return 0;
}

