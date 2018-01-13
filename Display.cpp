//THIS ONE

#include "Display.hpp"

#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texturefunctions.hpp"

//Global------------------------------------------------------------------------

bool g_firstMouse = true;
GLfloat g_lastX = 640 / 2.0;
GLfloat g_lastY = 480 / 2.0;
GLfloat g_xoffset = 640 / 2.0;
GLfloat g_yoffset = 480 / 2.0;
GLfloat g_yaw = -90.0f;
GLfloat g_pitch = 0.0f;
//Array in which the data for key presses is stored (used in key_callback())
bool g_key_data[1024];

//Private-----------------------------------------------------------------------

void Display::CreateQuad() {
  //Create four vertices covering the window
  float quad[] = {
    //Pos                 //UVs
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
     1.0f, -1.0f, 0.0f,   1.0f, 0.0f
  };

  //Generate arrays and buffers
  glGenVertexArrays(1, &(this->m_quadVAO));
  glGenBuffers(1, &(this->m_quadVBO));

  //Bind them together
  glBindVertexArray(this->m_quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_quadVBO);

  //Tell GL how to handle the buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);

  //Tell GL how to read buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0,                  //Index of the attribute
    3,                  //Number of components for the attribute
    GL_FLOAT,           //Type of values in attribute
    GL_FALSE,           //Are they normalized?
    5 * sizeof(float),  //In the array, how long steps should be taken for each element?
    (void*)0          //Inside the element, how big is the offset to the relevant values?
  );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1,                          //Index of the attribute
    2,                          //Number of components for the attribute
    GL_FLOAT,                   //Type of values in attribute
    GL_FALSE,                   //Are they normalized?
    5 * sizeof(float),          //In the array, how long steps should be taken for each element?
    (void*)(3 * sizeof(float))  //Inside the element, how big is the offset to the relevant values?
  );
}

void Display::RenderQuad() {
  glBindVertexArray(this->m_quadVAO);     //Bind the quad
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  //Draw the stuff
  glBindVertexArray(0);                   //Unbind the quad
}

void Display::FixLightUniforms(Shader* shader_ptr, std::string pnt_str, std::string dir_str, std::string spt_str, int n_pnt, int n_dir, int n_spt) {
  //Try to allocate space in shader for n lights of a type
  for (int i = 0; i < n_pnt; i++) {
    shader_ptr->FindUniformPntLightLoc(pnt_str, i); //SEND IN NAMES
  }

  for (int i = 0; i < n_dir; i++) {
    shader_ptr->FindUniformDirLightLoc(dir_str, i);
  }

  for (int i = 0; i < n_spt; i++) {
    shader_ptr->FindUniformSptLightLoc(spt_str, i);
  }
}

void Display::UploadLightPack(Shader* shader_ptr, LightPack& lPack) {
  //If there are lights saved in the vector pass them to shader and upload them
  for (GLuint i = 0; i < lPack.s_pnt_lights.size(); i++) {
    shader_ptr->UploadPntLight(lPack.s_pnt_lights[i], i);
  }

  for (GLuint i = 0; i < lPack.s_dir_lights.size(); i++) {
    shader_ptr->UploadDirLight(lPack.s_dir_lights[i], i);
  }

  for (GLuint i = 0; i < lPack.s_spt_lights.size(); i++) {
    shader_ptr->UploadSptLight(lPack.s_spt_lights[i], i);
  }
}

void Display::FixTextureUniforms(Shader* shader_ptr, std::string type_str, int n_tex) {
  //Try to allocate space in shader for n textures of a type

  for (int i = 0; i < n_tex; i++) {
    shader_ptr->FindUniformTextureLoc(type_str, i);
  }
}

//void Display::UploadTexture(Shader* shader_ptr, GLuint tex_id, int index) {
//  shader_ptr->UploadTexture(tex_id, index);
//}

//Public------------------------------------------------------------------------

Display::Display(int width, int height, const std::string& title, Camera* camPtr, Terrain* terrainPtr) {

  // start GL context and O/S window using the GLFW helper library
  if (!glfwInit()) {
    std::cout << "ERROR: could not start GLFW3" << std::endl;
    return;
  }
  // Set appropriate openGL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //Set width and height
  this->m_width = width;
  this->m_height = height;

  // Create a window
  m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!m_window) {
    std::cout << "ERROR: could not open window with GLFW3\n" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(m_window);

  // Bind camera to display
  m_camPtr = camPtr;
  m_camPtr2 = nullptr;
  m_camSwap = false;

  // Bind terrain
  m_terrain = terrainPtr;

  //Set the window to capture the mouse and hide it. It's like a cat but digital
  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  //Set callback functions
  glfwSetKeyCallback(m_window, KeyCallback);			      // This sets the basic key_callback function 'key_callback' as default.
  glfwSetCursorPosCallback(m_window, MouseCallback);		// This sets the basic mouse_callback function 'mouse_callback' as default.

  // second argument could be a struct of relevant pointers to control in callbackfunctions down below
  glfwSetWindowUserPointer(m_window, this);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); // version as a string
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "Version: " << version << std::endl;

  m_deltaTime = 0.0f;
  m_lastFrame = 0.0f;

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //Now avoid that cute little segmentation fault by initilizing the gbuffer here instead
  this->m_gBuffer.InitGBuffer();

  //Create a quad filling the entire screen to render textures onto
  this->CreateQuad();

} //Display::Display()

Display::~Display() {
  glfwTerminate();
}

bool Display::IsClosed() {
  return this->m_isClosed;
}

void Display::SetExtraCamera(Camera* camPtr) {
  m_camPtr2 = camPtr;
}

void Display::SetTerrain(Terrain *terrainPtr) {
  m_terrain = terrainPtr;
}

void Display::ToggleCamera() {
  if (m_camPtr2 != nullptr) {
    m_camSwap = !m_camSwap;
    std::cout << "=== Switching camera === " << (m_camSwap ? "2" : "1") << '\n';
    Camera* temp = m_camPtr;
    m_camPtr = m_camPtr2;
    m_camPtr2 = temp;
  }
}

void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SetShader(Shader* shaderPtr) {
  //NTS: Function specific for this program

  m_shaderPtr = shaderPtr;

  //Locate space in shader for matrices
  m_shaderPtr->FindUniformMatrixLoc("model");
  m_shaderPtr->FindUniformMatrixLoc("view");
  m_shaderPtr->FindUniformMatrixLoc("perspective");
  m_shaderPtr->FindUniformVec3Loc("camPos");

  //Locate space in shader for lights
  this->FixLightUniforms(m_shaderPtr, "pnt_lights", "dir_lights", "spt_lights", 1, 0, 0);
}

void Display::Update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
  // Set frame time
  GLfloat currentFrame = glfwGetTime();
  m_deltaTime = currentFrame - m_lastFrame;
  m_lastFrame = currentFrame;

  if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    m_isClosed = true;
  }

  // m_camPtr->UpdateCameraVectors();
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  glm::mat4 viewMatrix = m_camPtr->GetViewMatrix();
  glm::mat4 persMatrix = m_camPtr->GetPersMatrix();
  glm::vec3 camPos = m_camPtr->GetPosition();
  m_shaderPtr->UploadMatrix(modelMatrix, 0);
  m_shaderPtr->UploadMatrix(viewMatrix, 1);
  m_shaderPtr->UploadMatrix(persMatrix, 2);
  m_shaderPtr->UploadVec3(camPos, 0); // first index of vec3uniform vector in Shader class
}

void Display::Draw(ModelData& modelData, LightPack& lPack) {
  if (modelData.s_insideFrustum) {
    glUseProgram(m_shaderPtr->GetProgram());

    this->UploadLightPack(m_shaderPtr, lPack);

    m_shaderPtr->UploadMatrix(modelData.s_modelMat, 0);
    RenderMesh(&modelData);
  }
}

void Display::Draw(std::vector<ModelData*> modelPack, LightPack& lPack) {
    // Modelpack contains all models inside frustum
    glUseProgram(m_shaderPtr->GetProgram());

    UploadLightPack(m_shaderPtr, lPack);
    for (GLuint i = 0; i < modelPack.size(); i++) {
      m_shaderPtr->UploadMatrix(modelPack[i]->s_modelMat, 0);
      RenderMesh(modelPack[i]);
    }
}

void Display::RenderMesh(ModelData* modelData) {
  for (GLuint i = 0; i < modelData->s_meshIndices.size(); i++) {
    glBindVertexArray(modelData->s_VAOs[i]);
    glDrawElements(modelData->s_mode, modelData->s_meshIndices[i].size(), GL_UNSIGNED_INT, 0);
  }
}

void Display::SetDRShaders(Shader* geoS, Shader* lgtS) {
  //NTS: Fuction specific for this program

  this->m_geoShaderPtr = geoS;
  this->m_lgtShaderPtr = lgtS;

  //USE GEO-SHADER--------------------------------------------------------------
  glUseProgram(this->m_geoShaderPtr->GetProgram());

  //Locate space in shader for matrices
  this->m_geoShaderPtr->FindUniformMatrixLoc("model");
  this->m_geoShaderPtr->FindUniformMatrixLoc("view");
  this->m_geoShaderPtr->FindUniformMatrixLoc("perspective");
  this->m_geoShaderPtr->FindUniformMatrixLoc("light_mat");
  this->m_geoShaderPtr->FindUniformVec3Loc("camPos");

  //Locate space for the texures of models
  this->FixTextureUniforms(this->m_geoShaderPtr, "diffuse", 1);
  this->FixTextureUniforms(this->m_geoShaderPtr, "specular", 1);

  //USE LIGHT-SHADER------------------------------------------------------------
  glUseProgram(this->m_lgtShaderPtr->GetProgram());

  //Locate space for camera position
  this->m_lgtShaderPtr->FindUniformVec3Loc("camPos");

  //Locate space in shader for textures
  this->m_gBuffer.FindUniformSamplerLocs(this->m_lgtShaderPtr->GetProgram());
  this->m_gBuffer.UploadUniformSamplers();

  //Locate space in shader for texture_specular0
  this->m_lBuffer.FindUniformSamplerLocs(this->m_lgtShaderPtr->GetProgram());
  this->m_lBuffer.UploadUniformSamplers();

  //Locate space in shader for lights
  this->FixLightUniforms(this->m_lgtShaderPtr, "pnt_lights", "dir_lights", "spt_lights", 1, 1, 1);

}

void Display::SetComputeShader(Shader* comS, Shader* tarS) {
  //NTS: Function specific for this program
  GLint uni_loc;

  //Set the pointer to the compute shader
  this->m_comShaderPtr = comS;

  //USE COMPUTE-SHADER
  glUseProgram(this->m_comShaderPtr->GetProgram());

  uni_loc = this->m_comShaderPtr->GetUniform("xORy");
  this->FixTextureUniforms(this->m_comShaderPtr, "source", 1);
  this->FixTextureUniforms(this->m_comShaderPtr, "target", 1);

  //Initialize the PingPongBuffer
  this->m_ppBuffer.InitPingPongBuffer(this->m_width, this->m_height, uni_loc);

  //Set the uniform that the result should be sent to
  glUseProgram(tarS->GetProgram());
  this->FixTextureUniforms(tarS, "computed", 1);
  //tarS->FindUniformTextureLoc(type_str, i);
  //glUniform1i(tarS->GetUniform("texture_computed0"), COMPUTE_TEX);
}

void Display::SetShadowShader(Shader* shaS) {
  //NTS: Function specific for this program

  //Set the pointer to the shadow shader
  this->m_shaShaderPtr = shaS;

  //USE SHADOW-SHADER
  glUseProgram(this->m_shaShaderPtr->GetProgram());

  //WIP: Fix location for the  sptLight's matrix
  this->m_shaShaderPtr->FindUniformMatrixLoc("model");
  this->m_shaShaderPtr->FindUniformMatrixLoc("light_mat");

  this->m_lBuffer.InitLBuffer();

}

void Display::UpdateDR() {
  //std::cout << "In UpdateDR" << '\n';

  glfwSwapBuffers(this->m_window);
  glfwPollEvents();

  // Set frame time
  GLfloat currentFrame = glfwGetTime();
  this->m_deltaTime = currentFrame - this->m_lastFrame;
  this->m_lastFrame = currentFrame;

  if (glfwGetKey(this->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    this->m_isClosed = true;
  }

  this->m_camPos = m_camPtr->GetPosition();

  if (m_camPos.x > 0 && m_camPos.z > 0 && m_terrain != nullptr) {
    GLfloat heightLimit = m_terrain->GetHeight(m_camPos.x, m_camPos.z);
    //m_camPtr->ApplyGravity(heightLimit, m_deltaTime);
  }

  this->m_view = this->m_camPtr->GetViewMatrix();
  this->m_pers = this->m_camPtr->GetPersMatrix();
}

void Display::DrawDR(ModelData& modelData, LightPack& lPack) {
  // std::cout << "In DrawDR" << '\n';
  /*########## GEOMETRY PASS #################################################*/
  //Select program to use and bind framebuffer
  glUseProgram(this->m_geoShaderPtr->GetProgram());
  this->m_gBuffer.PrepGeoPass();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Upload the model matrix for the model and loop through all meshes
  this->m_geoShaderPtr->UploadMatrix(modelData.s_modelMat, 0);
  this->m_geoShaderPtr->UploadMatrix(this->m_view, 1);
  this->m_geoShaderPtr->UploadMatrix(this->m_pers, 2);

  for (GLuint i = 0; i < modelData.s_meshIndices.size(); i++) {
    glBindVertexArray(modelData.s_VAOs[i]);

    //Upload mesh textures
    int n_tex = 0;        //Varable tracking how many textures were found
    if (modelData.s_meshTextures.size() > 0) {
      switch (modelData.s_meshTextures[i].size()) {
        case 3:
          Bind2DTextureTo(modelData.s_meshTextures[i][2].id, NORMALMAP_TEX);
          n_tex++;
        case 2:
          //NTS:  The below line SHOULD NOT BE USED. The uniform is linked to a binding from where it gets its values
          //      Uploading to this uniform messes with how it is read. Line (And function) left for future reference.
          //this->UploadTexture(this->m_geoShaderPtr, modelData.s_meshTextures[i][1].id, 1);    //Specular
          Bind2DTextureTo(modelData.s_meshTextures[i][1].id, MESHSPEC_TEX);
          n_tex++;
        case 1:
          //this->UploadTexture(this->m_geoShaderPtr, modelData.s_meshTextures[i][0].id, 0);    //Diffuse
          Bind2DTextureTo(modelData.s_meshTextures[i][0].id, MESHDIFF_TEX);
          n_tex++;
          break;
        default:
          //No textures in mesh
          break;
      }
    }
    this->m_geoShaderPtr->DirectInt("n_tex", n_tex);  //Variable uploaded to shader for checking if the uniform samplers contain anything

    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawElements(GL_TRIANGLES, modelData.s_meshIndices[i].size(), GL_UNSIGNED_INT, 0);
  }

  //Unbind framebuffer after render
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*########## COMPUTE PASS ##################################################*/
  glUseProgram(this->m_comShaderPtr->GetProgram());

  this->m_ppBuffer.DoPingPong(10, this->m_gBuffer.GetColTextureId());

  /*########## LIGHT PASS ####################################################*/
  //Select the program to use and load up the gBuffer textures
  glUseProgram(this->m_lgtShaderPtr->GetProgram());

  this->m_gBuffer.PrepLightPass();
  this->m_ppBuffer.BindResult();

  //Upload all lights in the LightPack
  this->UploadLightPack(this->m_lgtShaderPtr, lPack);

  //Draw the quad filling the screen
  this->RenderQuad();

}

void Display::DrawDR(std::vector<ModelData*> modelPack, LightPack& lPack) {

  /*########## SHADOW PASS ###################################################*/

  //Set Viewport
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

  glUseProgram(this->m_shaShaderPtr->GetProgram());
  this->m_lBuffer.PrepShadowPass();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Upload the light matrix
  this->m_shaShaderPtr->UploadMatrix( lPack.s_spt_lights[0].getLightMat(), 1);

  //Loop through all models
  for (GLuint i = 0; i < modelPack.size(); i++) {
      this->m_shaShaderPtr->UploadMatrix(modelPack[i]->s_modelMat, 0);
      RenderMeshDR(modelPack[i]);
  }

  //Unbind framebuffer after render
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  /*########## GEOMETRY PASS #################################################*/

  //Set Viewport
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  //Select program to use and bind framebuffer
  glUseProgram(this->m_geoShaderPtr->GetProgram());
  this->m_gBuffer.PrepGeoPass();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Upload the view and perspective matrices
  this->m_geoShaderPtr->UploadMatrix(this->m_view, 1);
  this->m_geoShaderPtr->UploadMatrix(this->m_pers, 2);

  //Upload camera position
  this->m_geoShaderPtr->UploadVec3(this->m_camPos, 0);

  //Upload the light matrix
  this->m_geoShaderPtr->UploadMatrix( lPack.s_spt_lights[0].getLightMat(), 3);

  for (GLuint i = 0; i < modelPack.size(); i++) {
      this->m_geoShaderPtr->UploadMatrix(modelPack[i]->s_modelMat, 0);
      RenderMeshDR(modelPack[i]);
  }

  //Unbind framebuffer after render
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*########## COMPUTE PASS ##################################################*/
  glUseProgram(this->m_comShaderPtr->GetProgram());

  this->m_ppBuffer.DoPingPong(10, this->m_gBuffer.GetColTextureId());

  /*########## LIGHT PASS ####################################################*/
  //Select the program to use and load up the gBuffer textures
  glUseProgram(this->m_lgtShaderPtr->GetProgram());

  this->m_gBuffer.PrepLightPass();
  this->m_ppBuffer.BindResult();
  this->m_lBuffer.PrepLightPass();

  //Upload camera position
  this->m_geoShaderPtr->UploadVec3(this->m_camPos, 0);

  //Upload all lights in the LightPack
  this->UploadLightPack(this->m_lgtShaderPtr, lPack);

  //Draw the quad filling the screen
  this->RenderQuad();
}

void Display::RenderMeshDR(ModelData* modelData) {
  for (GLuint i = 0; i < modelData->s_meshIndices.size(); i++) {
    glBindVertexArray(modelData->s_VAOs[i]);

    //Upload mesh textures
    int n_tex = 0;        //Varable tracking how many textures were found
      switch (modelData->s_meshTextures[i].size()) {
        case 3:
          Bind2DTextureTo(modelData->s_meshTextures[i][0].id, MESHDIFF_TEX);
          Bind2DTextureTo(modelData->s_meshTextures[i][1].id, MESHSPEC_TEX);
          Bind2DTextureTo(modelData->s_meshTextures[i][2].id, NORMALMAP_TEX);
          n_tex = 3;
          break;
        case 2:
          //NTS:  The below line SHOULD NOT BE USED. The uniform is linked to a binding from where it gets its values
          //      Uploading to this uniform messes with how it is read. Line (And function) left for future reference.
          //this->UploadTexture(this->m_geoShaderPtr, modelData.s_meshTextures[i][1].id, 1);    //Specular
          Bind2DTextureTo(modelData->s_meshTextures[i][0].id, MESHDIFF_TEX);
          Bind2DTextureTo(modelData->s_meshTextures[i][1].id, MESHSPEC_TEX);
          n_tex = 2;
          break;
        case 1:
          //this->UploadTexture(this->m_geoShaderPtr, modelData.s_meshTextures[i][0].id, 0);    //Diffuse
          Bind2DTextureTo(modelData->s_meshTextures[i][0].id, MESHDIFF_TEX);
          n_tex++;
          break;
        default:
          //No textures in mesh
          break;
      }
    // this->m_geoShaderPtr->DirectInt("n_tex", n_tex);  //Variable uploaded to shader for checking if the uniform samplers contain anything

    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawElements(GL_TRIANGLES, modelData->s_meshIndices[i].size(), GL_UNSIGNED_INT, 0);
  }
}

//GLOBAL------------------------------------------------------------------------

void KeyCallback(GLFWwindow* winPtr, int key, int scan, int act, int mode) {
  if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) {
    glfwSetWindowShouldClose(winPtr, GL_TRUE);
  }

  // ! setting global variable
  //Store the data from when a key is pressed
  if (key >= 0 && key < 1024)
  {
    if (act == GLFW_PRESS) {
      g_key_data[key] = true;
    }
    else if (act == GLFW_RELEASE) {
      g_key_data[key] = false;
    }
  }

  Display* d = (Display*)glfwGetWindowUserPointer(winPtr);
  if (g_key_data[GLFW_KEY_W])
    d->m_camPtr->ProcessKeyboard(FORWARD, d->m_deltaTime);
  if (g_key_data[GLFW_KEY_S])
    d->m_camPtr->ProcessKeyboard(BACKWARD, d->m_deltaTime);
  if (g_key_data[GLFW_KEY_A])
    d->m_camPtr->ProcessKeyboard(LEFT, d->m_deltaTime);
  if (g_key_data[GLFW_KEY_D])
    d->m_camPtr->ProcessKeyboard(RIGHT, d->m_deltaTime);
  if (g_key_data[GLFW_KEY_SPACE])
    d->m_camPtr->ProcessKeyboard(UP, d->m_deltaTime);

  if (g_key_data[GLFW_KEY_C])
    d->ToggleCamera();
}

void MouseCallback(GLFWwindow* winPtr, double xPos, double yPos) {
  if (g_firstMouse)
  {
    g_lastX = xPos;
    g_lastY = yPos;
    g_firstMouse = false;
  }

  g_xoffset = xPos - g_lastX;
  g_yoffset = g_lastY - yPos;

  g_lastX = xPos;
  g_lastY = yPos;

  Display* d = (Display*)glfwGetWindowUserPointer(winPtr);
  d->m_camPtr->ProcessMouseMovement(g_xoffset, g_yoffset);
}
