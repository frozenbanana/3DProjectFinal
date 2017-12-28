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
#include "Terrain.hpp"
<<<<<<< HEAD

#include "QuadTree.hpp"

=======
#include "QuadTree.hpp"
>>>>>>> CullNode implemented but not tested
//Trickster Domain
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* geometry_shader = "res/shaders/base_gs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

const char* geo_vs = "res/shaders/geoPass_vs.glsl";
//Add a gs here
const char* geo_fs = "res/shaders/geoPass_fs.glsl";
const char* lgt_vs = "res/shaders/lightPass_vs.glsl";
const char* lgt_fs = "res/shaders/lightPass_fs.glsl";

int main() {

  // SETUP VIEW
<<<<<<< HEAD
  Camera camera(glm::vec3(0.0f, 0.0f, -5.0f));

  std::string disName = "Display heyheyhey";
  Display display = Display(WINDOW_WIDTH, WINDOW_HEIGHT, disName, &camera);

  //Shader shader(vertex_shader, fragment_shader);
  //display.SetShader(&shader);   //Function also fixes uniforms for 3 matrices an a bunch of lights

  Shader geoShader(geo_vs, geo_fs);
  Shader lightShader(lgt_vs, lgt_fs);
  display.SetDRShaders(&geoShader, &lightShader);

  // SETUP MODELS
  Model model1("res/models/nano/nanosuit.obj");
  Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  model2.SetPos(glm::vec3(-10.0f, 5.0f, 0.0f));
  model2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

  // PACKAGE MODEL DATA TO DISPLAY (STATIC)
  ModelData modelData1= model1.GetModelData();
  ModelData modelData2 = model2.GetModelData();
  //ModelData modifiedModeldata1;

  // Frustum culling
  //glm::mat4 viewPers = camera.GetViewMatrix() * camera.GetPersMatrix();
  //Frustum frustum(viewPers);

=======
  Camera camera(glm::vec3(-3.0f, -6.0f, 8.3f));
  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, "Test terrain class", &camera);
  Shader shader(vertex_shader, geometry_shader, fragment_shader);
  display.SetShader(&shader);   //Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP MODELS
  Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
  Model model1("res/models/nano/nanosuit.obj");
  // Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  // model2.SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
  // model2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
  // // PACKAGE MODEL DATA
  ModelData modelData1 = model1.GetModelData();
  // ModelData modelData2 = model2.GetModelData();
  ModelData terrainData = terrain.GetModelData();
  Frustum frustum(camera.GetViewPersMatrix());
<<<<<<< HEAD
  
>>>>>>> Frustum culling working for real
=======
  QuadTree quadtree(128);
  quadtree.InsertModelInTree(modelData1);
>>>>>>> CullNode implemented but not tested
  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(glm::vec3(0.0f, 10.0f, 0.0f), COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

  //PACKAGE LIGHT DATA TO DISPLAY (STATIC)
  LightPack lPack = lightHandler.GetLightPack();

  display.Clear(0.0f, 0.20f, 0.1f, 1.0f);

  //DRAW LOOP
  while(!display.IsClosed()) {
<<<<<<< HEAD
    //display.Update();
    display.UpdateDR();

    //display.Draw(modelData1, lPack);
    //display.Draw(modelData2, lPack);
    display.DrawDR(modelData1, lPack);
    //display.DrawDR(modelData2, lPack);
=======
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
    frustum.SetFrustum(camera.GetViewPersMatrix());
    // frustum.CullMeshes(&modelData1); // modified
    // frustum.CullMeshes(&modelData2); // modified
    frustum.CullMeshes(&terrainData);   // modified
    // display.Draw(modelData1, lPack);
    // display.Draw(modelData2, lPack);
    display.Draw(terrainData, lPack);
    display.Update();
>>>>>>> Frustum culling working for real
  }

  return 0;
}
