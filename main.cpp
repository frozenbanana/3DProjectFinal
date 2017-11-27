#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include "Display.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Frustum.hpp"
#include "GLOBALS.hpp"

const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

ModelData CullMeshes(Frustum& frustum, ModelData modelData);

int main() {
  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 0.0f, -5.0f));
  Display display(WINDOW_HEIGHT, WINDOW_WIDTH, "Test Model class", &camera);
  Shader shader(vertex_shader, fragment_shader);
  display.SetShader(&shader);

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

  // DRAW LOOP
  while(!display.IsClosed()) {
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
    frustum.SetFrustum(camera.GetViewMatrix() * camera.GetPersMatrix());
    modifiedModeldata1 = CullMeshes(frustum, modelData1);
    display.Draw(modifiedModeldata1);
    display.Draw(modelData2);
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
