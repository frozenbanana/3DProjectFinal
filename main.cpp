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
#include "QuadTree.hpp"
//Trickster Domain
#include "GLOBALS.hpp"
#include "PackageStructs.hpp"

const char* vertex_shader = "res/shaders/base_vs.glsl";
const char* geometry_shader = "res/shaders/base_gs.glsl";
const char* fragment_shader = "res/shaders/base_fs.glsl";

const char* geo_vs = "res/shaders/geoPass_vs.glsl";
const char* geo_gs = "res/shaders/geoPass_gs.glsl";
const char* geo_fs = "res/shaders/geoPass_fs.glsl";

const char* pt_cs = "res/shaders/PTcompute.glsl";

const char* lgt_vs = "res/shaders/lightPass_vs.glsl";
const char* lgt_fs = "res/shaders/lightPass_fs.glsl";

int main() {

  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
  Camera camera2(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f);
  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, "Normal mapping", &camera);
  display.SetExtraCamera(&camera2);
  Shader shader(vertex_shader, geometry_shader, fragment_shader);
  display.SetShader(&shader);   // Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP QUADTREE AND FRUSTUM WITH MODELS
  Frustum frustum(camera.GetViewPersMatrix());
  QuadTree quadtree(QUADTREE_ROOT_WIDTH, QUADTREE_MIN_WIDTH);

  // SETUP DEFERRED RENDERING SHADERS
  Shader geoShader(geo_vs, geo_gs, geo_fs);
  Shader lightShader(lgt_vs, lgt_fs);
  display.SetDRShaders(&geoShader, &lightShader);

  // SETUP COMPUTE SHADER
  Shader comShader(pt_cs);
  display.SetComputeShader(&comShader, &lightShader);

  // SETUP MODELS
  // terrain
  Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
  terrain.SetTerrainTexture("normalmap/normal2.png", "texture_normal");
  display.SetTerrain(&terrain);
  // quadtree.InsertModelInTree(&terrain.GetModelData());

  // cubes
  Model cubes[21];
  glm::vec3 cubesPos[21] = {
        glm::vec3(  2.0f,  2.0f,  2.0f), // bottom left
        glm::vec3(  2.0f,  2.0f,  6.0f),
        glm::vec3(  2.0f,  2.0f,  10.0f),
        glm::vec3(  2.0f,  2.0f,  14.0f),
        glm::vec3(  2.0f,  2.0f,  18.0f),
        glm::vec3(  2.0f,  2.0f,  22.0f),
        glm::vec3(  2.0f,  2.0f,  26.0f),

        glm::vec3(  7.0f,  2.0f,  4.0f), // almost bottom left
        glm::vec3(  7.0f,  2.0f,  6.0f),
        glm::vec3(  7.0f,  2.0f,  10.0f),
        glm::vec3(  7.0f,  3.0f,  14.0f),
        glm::vec3(  7.0f,  3.0f,  18.0f),
        glm::vec3(  7.0f,  3.0f,  22.0f),
        glm::vec3(  7.0f,  3.0f,  26.0f),

        glm::vec3(  11.0f,  4.0f,  4.0f), // almost bottom left
        glm::vec3(  11.0f,  4.0f,  6.0f),
        glm::vec3(  11.0f,  4.0f,  10.0f),
        glm::vec3(  11.0f,  4.0f,  14.0f),
        glm::vec3(  11.0f,  4.0f,  18.0f),
        glm::vec3(  11.0f,  4.0f,  22.0f),
        glm::vec3(  11.0f,  4.0f,  26.0f),
  };
  quadtree.InsertModelInTree(&cubes[0].GetModelData());
  for (size_t i = 0; i < 21; i++) {
    cubes[i].LoadModel("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
    cubes[i].SetPos(cubesPos[i]);
    quadtree.InsertModelInTree(&cubes[i].GetModelData());
  }

  // Nano dude
  Model nanoDudes[3];
  glm::vec3 nanoDudesPos[3] = {
    glm::vec3(  25.0f,  4.0f,  25.0f), // bottom left
    glm::vec3(  30.0f,  4.0f,  10.0f),
    glm::vec3(  10.0f,  0.0f,  30.0f),
  };

  for (size_t i = 0; i < 3; i++) {
    nanoDudes[i].LoadModel("res/models/nano/nanosuit.obj");
    nanoDudes[i].SetPos(nanoDudesPos[i]);
    quadtree.InsertModelInTree(&nanoDudes[i].GetModelData());
  }

  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(glm::vec3(0.0f, 10.0f, 0.0f), COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

  // Vector to draw
  std::vector<ModelData*> modelsToDraw;

  // //PACKAGE LIGHT DATA TO DISPLAY (STATIC)
  LightPack lPack = lightHandler.GetLightPack();
  display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
  //DRAW LOOP
  while(!display.IsClosed()) {
     display.UpdateDR();
     frustum.SetFrustum(camera.GetViewPersMatrix());
     frustum.CullNode(quadtree.GetRootNode());        // recursivly cull every node in QuadTree
     quadtree.FillModelPack(quadtree.GetRootNode());  // recursivly fill modelPackage in QuadTree
     modelsToDraw = quadtree.GetModelPack();          // Add culled models
     modelsToDraw.push_back(&terrain.GetModelData()); // add always terrain
     display.DrawDR(modelsToDraw, lPack);             // Draw models

     quadtree.ClearModelPack();                      // reset modelpack for new culling
  }

  return 0;
}
