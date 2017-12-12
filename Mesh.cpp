#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
  m_vertices = vertices;
  m_indices = indices;

  // All data is set. Time to setup mesh
    SetupMesh();
  }

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
  m_vertices = vertices;
  m_indices = indices;
  m_textures = textures;

  // All data is set. Time to setup mesh
    SetupMesh();
  }

  void Mesh::SetupMesh() {
    // Create buffers for this mesh
    glGenBuffers(1 , &m_vbo);
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1 , &m_ebo);

    // Bind vao and vbo
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Write vertices data to vbo
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

  // Bind ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // Write indices to ebo
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

  // Configure vao
  // Vertex position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
  // Vertex normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));
  // Vertex texture
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2*sizeof(glm::vec3)));
 }

std::vector<glm::vec3> Mesh::GetPos() {
  std::vector<glm::vec3> pos;
  for (GLuint i = 0; i < m_vertices.size(); i++) {
    pos.push_back(m_vertices[i].GetPos());
  }
  return pos;
}

GLuint Mesh::GetVAO() { return m_vao; }
GLuint Mesh::GetVBO() { return m_vbo; }
GLuint Mesh::GetEBO() { return m_ebo; }

Mesh::~Mesh() {}
