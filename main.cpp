// testbox2
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include "Display.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "Model.hpp"
#include "Shader.hpp"

const char* vertex_shader =
  "#version 440\n"
  "in vec3 vp;"
  "void main() {"
  "  gl_Position = vec4(vp, 1.0);"
  "}";

const char* fragment_shader =
  "#version 440\n"
  "out vec4 FragColor;"
  "void main() {"
  "  FragColor = vec4(0.5, 0.0, 0.5, 1.0);"
  "}";


int main() {
  Display display(640, 480, "Test Model class");
  Model model("res/models/nano/nanosuit.obj");

  /* OTHER STUFF GOES HERE NEXT */
  GLfloat points[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  // Check if shader where compiled fine
  int  success;
  char infoLog[512];

  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if(!success) {
      glGetShaderInfoLog(vs, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fs, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vs);
  glAttachShader(shader_program, fs);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  std::vector<GLuint> vaos = model.GetVAOs();

  while(!display.IsClosed()) {
    // wipe the drawing surface clear
    display.Clear(0.0f, 0.20f, 0.1f, 1.0f);
    glUseProgram(shader_program);
    // glBindVertexArray(vao);
    for (int i = 0; i < model.GetModelMeshes().size(); i++) {
    glBindVertexArray(vaos[i]);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, model.GetModelMeshes()[i].m_indices.size(), GL_UNSIGNED_INT, 0);
    }
    // update other events like input handling
    // put the stuff we've been drawing onto the display
    display.Update();
  }

  return 0;
}
