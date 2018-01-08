//THIS ONE

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


#include "LightHandler.hpp"
#include "Terrain.hpp"

#include "QuadTree.hpp"

//Trickster Domain
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* geometry_shader = "res/shaders/base_gs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

<<<<<<< HEAD
<<<<<<< HEAD
const char* geo_vs = "res/shaders/geoPass_vs.glsl";
//Add a gs here
const char* geo_fs = "res/shaders/geoPass_fs.glsl";
const char* lgt_vs = "res/shaders/lightPass_vs.glsl";
const char* lgt_fs = "res/shaders/lightPass_fs.glsl";
=======
>>>>>>> Frustum not working yet
=======
// DEFERRED RENDERING SHADERS
// geo shader
const char* geo_vs = "res/shaders/geoPass_vs.glsl";
//Add a gs here
const char* geo_fs = "res/shaders/geoPass_fs.glsl";
// light shader
const char* lgt_vs = "res/shaders/lightPass_vs.glsl";
const char* lgt_fs = "res/shaders/lightPass_fs.glsl";

// COMPUTE SHADER
const char* pt_cs = "res/shaders/PTcompute.glsl";
>>>>>>> comiitng

int main() {
  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
  Camera camera2(glm::vec3(0.0f, 7.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f);
  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, "Test frustum culling with quadtree", &camera);
  display.SetExtraCamera(&camera2);
  Shader shader(vertex_shader, geometry_shader, fragment_shader);
  display.SetShader(&shader);   // Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP QUADTREE AND FRUSTUM WITH MODELS
  Frustum frustum(camera.GetViewPersMatrix());
  QuadTree quadtree(QUADTREE_ROOT_WIDTH, QUADTREE_MIN_WIDTH);
  // quadtree.InsertModelInTree(&modelData1);
  // quadtree.InsertModelInTree(&modelData2);

  Shader geoShader(geo_vs, geo_fs);
  Shader lightShader(lgt_vs, lgt_fs);
  display.SetDRShaders(&geoShader, &lightShader);

  // SETUP MODELS
<<<<<<< HEAD
<<<<<<< HEAD
  Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
  // Model model1("res/models/nano/nanosuit.obj");
<<<<<<< HEAD
  // Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  // model2.SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
  // model2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
  // // PACKAGE MODEL DATA
  // ModelData modelData1 = model1.GetModelData();
  // ModelData modelData2 = model2.GetModelData();
  ModelData terrainData = terrain.GetModelData();
  Frustum frustum(camera.GetViewPersMatrix());

=======
  // Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
=======
  Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
>>>>>>> Frustum not working yet
  Model model1("res/models/nano/nanosuit.obj");
  model1.SetPos(glm::vec3(0.0f, 0.0f, 5.0f));
  model1.SetScale(glm::vec3(.5f, .5f, 0.5f));
  Model model2("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  model2.SetPos(glm::vec3(15.0f, 0.0f, 15.0f));
  model2.SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
=======
  // model1.SetPos(glm::vec3(0.0f, 0.0f, 6.0f));
  // model1.SetScale(glm::vec3(.5f, .5f, 0.5f));
  // Model model2("res/models/cube/cube_g14.26

  glm::vec3 cubePos[21] = {
        glm::vec3(  2.0f,  0.0f,  2.0f), // bottom left
        glm::vec3(  2.0f,  0.0f,  6.0f),
        glm::vec3(  2.0f,  0.0f,  10.0f),
        glm::vec3(  2.0f,  0.0f,  14.0f),
        glm::vec3(  2.0f,  0.0f,  18.0f),
        glm::vec3(  2.0f,  0.0f,  22.0f),
        glm::vec3(  2.0f,  0.0f,  26.0f),

        glm::vec3(  7.0f,  0.0f,  4.0f), // almost bottom left
        glm::vec3(  7.0f,  0.0f,  6.0f),
        glm::vec3(  7.0f,  0.0f,  10.0f),
        glm::vec3(  7.0f,  0.0f,  14.0f),
        glm::vec3(  7.0f,  0.0f,  18.0f),
        glm::vec3(  7.0f,  0.0f,  22.0f),
        glm::vec3(  7.0f,  0.0f,  26.0f),

        glm::vec3(  11.0f,  0.0f,  4.0f), // almost bottom left
        glm::vec3(  11.0f,  0.0f,  6.0f),
        glm::vec3(  11.0f,  0.0f,  10.0f),
        glm::vec3(  11.0f,  0.0f,  14.0f),
        glm::vec3(  11.0f,  0.0f,  18.0f),
        glm::vec3(  11.0f,  0.0f,  22.0f),
        glm::vec3(  11.0f,  0.0f,  26.0f),
  };

  Model cubeModels[21];
  for (size_t i = 0; i < 21; i++) {
    cubeModels[i].LoadModel("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
    cubeModels[i].SetPos(cubePos[i]);
    quadtree.InsertModelInTree(&cubeModels[i].GetModelData());
  }
>>>>>>> Frustum is still working!

  // PACKAGE MODEL DATA
  // ModelData modelData1 = model1.GetModelData();
  // ModelData modelData2 = model2.GetModelData();
  ModelData terrainData = terrain.GetModelData();

<<<<<<< HEAD
  // SETUP QUADTREE AND FRUSTUM WITH MODELS
  Frustum frustum(camera.GetViewPersMatrix());
  QuadTree quadtree(QUADTREE_ROOT_WIDTH, QUADTREE_MIN_WIDTH);
  // quadtree.InsertModelInTree(&modelData1);
  quadtree.InsertModelInTree(&modelData2);
>>>>>>> It culls not somewhat OK
=======
>>>>>>> Frustum is still working!
  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(glm::vec3(0.0f, 10.0f, 0.0f), COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

  // //PACKAGE LIGHT DATA TO DISPLAY (STATIC)
  LightPack lPack = lightHandler.GetLightPack();

<<<<<<< HEAD
  display.Clear(0.0f, 0.20f, 0.1f, 1.0f);

=======
>>>>>>> commiting test (21 cubes)
  //DRAW LOOP
  while(!display.IsClosed()) {
<<<<<<< HEAD
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
    frustum.SetFrustum(camera.GetViewPersMatrix());
    // frustum.CullMeshes(&modelData1); // modified
    // frustum.CullMeshes(&modelData2); // modified
    frustum.CullMeshes(&terrainData);   // modified
    // display.Draw(modelData1, lPack);
    // display.Draw(modelData2, lPack);
    display.Draw(terrainData, lPack);
    display.Update();
=======
     display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
     frustum.SetFrustum(camera.GetViewPersMatrix());
     // frustum.CullMeshes(&modelData2);        // recursivly cull every node in QuadTree
     frustum.CullNode(quadtree.GetRootNode());        // recursivly cull every node in QuadTree
     quadtree.FillModelPack(quadtree.GetRootNode());  // recursivly fill modelPackage in QuadTree
     // if (quadtree.GetModelPack().size() > 0)
     //   std::cout << quadtree.GetModelPack()[0]->s_insideFrustum << std::endl;
     display.Draw(quadtree.GetModelPack(), lPack);   // Draw  culled models
     // display.Draw(modelData2, lPack);
     // display.Draw(terrainData, lPack);

     display.Update();
     quadtree.ClearModelPack();                      // reset modelpack for new culling
>>>>>>> It culls not somewhat OK
  }

  return 0;
}
