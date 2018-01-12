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
const char* lgt_vs = "res/shaders/lightPass_vs.glsl";
const char* lgt_fs = "res/shaders/lightPass_fs.glsl";

const char* gau_cs = "res/shaders/PTcompute.glsl";

const char* sha_vs = "res/shaders/shadowPass_vs.glsl";
const char* sha_fs = "res/shaders/shadowPass_fs.glsl";

int main() {

  // SETUP VIEW
  Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
  Camera camera2(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f);
  Display display(WINDOW_WIDTH, WINDOW_HEIGHT, "Integrating frustum and DR", &camera);
  display.SetExtraCamera(&camera2);
  //Shader shader(vertex_shader, geometry_shader, fragment_shader);
  //display.SetShader(&shader);   // Function also fixes uniforms for 3 matrices an a bunch of lights

  // SETUP QUADTREE AND FRUSTUM WITH MODELS
  Frustum frustum(camera.GetViewPersMatrix());
  QuadTree quadtree(QUADTREE_ROOT_WIDTH, QUADTREE_MIN_WIDTH);

  // SETUP DEFERRED RENDERING SHADERS
  Shader geoShader(geo_vs, geo_gs, geo_fs);
  Shader lightShader(lgt_vs, lgt_fs);
  display.SetDRShaders(&geoShader, &lightShader);

  // SETUP COMPUTE SHADER
  Shader comShader(gau_cs);
  display.SetComputeShader(&comShader, &lightShader);

  // SETUP SHADOW SHADER
  Shader shaShader(sha_vs, sha_fs);
  display.SetShadowShader(&shaShader);

  // SETUP MODELS
  // terrain
  Terrain terrain("res/heightmap/example/BMP_example.bmp", 20);
  display.SetTerrain(&terrain);
  // quadtree.InsertModelInTree(&terrain.GetModelData());

  // cubes
  Model cubes[23];
  glm::vec3 cubesPos[23] = {
        glm::vec3(  2.0f,     15.0f,  2.0f), // Spread
        glm::vec3(  76.0f,    13.0f,  50.0f),
        glm::vec3(  145.0f,   19.0f,  85.0f),
        glm::vec3(  203.0f,   15.0f,  133.0f),
        glm::vec3(  289.0f,   14.0f,  169.0f),
        glm::vec3(  349.0f,   12.0f,  142.0f),
        glm::vec3(  410.0f,   13.0f,  63.0f),

        glm::vec3(  175.0f,   19.0f,  322.0f), // Spread
        glm::vec3(  62.0f,    13.0f,  212.0f),
        glm::vec3(  245.0f,   19.0f,  181.0f),
        glm::vec3(  101.0f,   16.0f,  249.0f),
        glm::vec3(  465.0f,   15.0f,  199.0f),
        glm::vec3(  372.0f,   12.0f,  333.0f),
        glm::vec3(  9.0f,     18.0f,  326.0f),

        glm::vec3(  9.0f,   5.0f,  9.0f), // Corner square
        glm::vec3(  9.0f,   5.0f,  18.0f),
        glm::vec3(  9.0f,   5.0f,  27.0f),

        glm::vec3(  18.0f,  5.0f,  9.0f),
        glm::vec3(  18.0f,  5.0f,  18.0f),
        glm::vec3(  18.0f,  5.0f,  27.0f),

        glm::vec3(  27.0f,  5.0f,  9.0f),
        glm::vec3(  27.0f,  5.0f,  18.0f),
        glm::vec3(  27.0f,  5.0f,  27.0f),
  };
  quadtree.InsertModelInTree(&cubes[0].GetModelData());
  for (size_t i = 0; i < 23; i++) {
    cubes[i].LoadModel("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
    cubes[i].SetPos(cubesPos[i]);
    quadtree.InsertModelInTree(&cubes[i].GetModelData());
  }

  // Nano dude
  Model nanoDudes[3];
  glm::vec3 nanoDudesPos[3] = {
    glm::vec3(  85.0f,    4.0f,   85.0f), // bottom left
    glm::vec3(  155.0f,   4.0f,   155.0f),
    glm::vec3(  425.0f,   0.0f,   425.0f),
  };

  for (size_t i = 0; i < 1; i++) {
    nanoDudes[i].LoadModel("res/models/nano/nanosuit.obj");
    nanoDudes[i].SetPos(nanoDudesPos[i]);
    quadtree.InsertModelInTree(&nanoDudes[i].GetModelData());
  }

  // SETUP lights
  LightHandler lightHandler;
  lightHandler.AddPntLight(
    glm::vec3(30.0f, 30.0f, 30.0f),
    COLOR_WHITE,
    COLOR_BLUE,
    COLOR_BLUE
  );
  Model pnt_light_cube("res/models/cube/cube_green_phong_12_tris_QUADS.obj");
  pnt_light_cube.SetPos(glm::vec3(30.0, 30.0, 30.0));
  pnt_light_cube.SetPos(glm::vec3(0.5, 0.5, 0.5));

  quadtree.InsertModelInTree(&pnt_light_cube.GetModelData());

  lightHandler.AddDirLight(
    glm::vec3(-1.0, -1.0, 1.0),
    COLOR_WHITE,
    COLOR_GREEN,
    COLOR_GREEN
  );
  lightHandler.AddSptLight(
    glm::vec3(10.0f, 50.0f, 10.0f),
    glm::vec3(20.0f, 0.0f, 5.0f),
    0.1f,
    120.0f,
    COLOR_WHITE,
    COLOR_RED,
    COLOR_RED
  );

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
