#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>

//View Domain
#include "Display.hpp"
#include "Shader.hpp"

#include "Frustum.hpp"

//Model Domain
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Model.hpp"

#include "LightHandler.hpp"

//Trickster Domain
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"



const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

ModelData CullMeshes(Frustum& frustum, ModelData modelData);

int main() {
  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 0.0f, -5.0f));
  Display display(WINDOW_HEIGHT, WINDOW_WIDTH, "Test Model class", &camera);
  Shader shader(vertex_shader, fragment_shader);
  display.SetShader(&shader);   //Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP MODELS
  Model model("res/models/nano/nanosuit.obj");
  Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  model2.SetPos(glm::vec3(-10.0f, 5.0f, 0.0f));
  model2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

  // PACKAGE MODEL DATA TO DISPLAY (STATIC)
  ModelData modelData1= model.GetModelData();
  ModelData modelData2 = model2.GetModelData();
  ModelData modifiedModeldata1;

  // Frustum culling
  glm::mat4 viewPers = camera.GetViewMatrix() * camera.GetPersMatrix();
  Frustum frustum(viewPers);

  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(glm::vec3(0.0f, 10.0f, 0.0f), COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

  //PACKAGE LIGHT DATA TO DISPLAY (STATIC)
  LightPack lPack = lightHandler.GetLightPack();

  // DRAW LOOP
  while(!display.IsClosed()) {
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
//<<<<<<< HEAD
//    frustum.SetFrustum(camera.GetViewMatrix() * camera.GetPersMatrix());
//    modifiedModeldata1 = CullMeshes(frustum, modelData1);
//    display.Draw(modifiedModeldata1);
//    display.Draw(modelData2);
//=======
    //display.Draw(modelData);
    display.Draw(modelData2, lPack);
//>>>>>>> 529c64269eae889871fb5b850a5ef441a0668898
    display.Update();
  }

  return 0;
}

ModelData CullMeshes(Frustum& frustum, ModelData modelData) {
  bool meshIsInside = false;
  for(GLuint i = 0; i < modelData.s_meshPos.size(); i++) {
    // Check every point in current mesh
    for(GLuint j = 0; j < modelData.s_meshPos[i].size() && !meshIsInside; j++) {
      meshIsInside = frustum.InsideFrustrum(modelData.s_meshPos[i][j]);
    }
    // if no point in mesh is in frustum it remove it from stuff to be rendered
    if(!meshIsInside) {
      modelData.s_VAOs.erase(modelData.s_VAOs.begin() + i);
      modelData.s_meshIndices.erase(modelData.s_meshIndices.begin() + i);
    }
  }
  return modelData;
}
