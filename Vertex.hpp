#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>

class Vertex
{
public:
  Vertex() {
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
    m_texCoord = glm::vec2(0.0f, 0.0f);
    m_tangent = glm::vec3(0.0f, 0.0f, 0.0f);
    m_bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  Vertex(const glm::vec3& pos, const glm::vec2& texCoord,
          const glm::vec3& normal = glm::vec3(0.f, 0.f, 0.f),
          const glm::vec3& tangent = glm::vec3(0.f, 0.f, 0.f),
          const glm::vec3& bitanget = glm::vec3(0.f, 0.f, 0.f)) {
    m_pos = pos;
    m_normal = normal;
    m_texCoord = texCoord;
    m_tangent = tangent;
    m_bitangent = bitanget;
  }

  void SetPos(const glm::vec3& pos) { m_pos = pos;};
  void SetNormal(const glm::vec3 normal) { m_normal = normal;};
  void SetTexCoord(const glm::vec2 texCoord) { m_texCoord = texCoord;};
  void SetTangent(const glm::vec3 tangent) { m_tangent = tangent;};
  void SetBitangent(const glm::vec3 bitangent) { m_bitangent = bitangent;};
  inline glm::vec3 GetPos() { return m_pos; };
  inline glm::vec3 GetNormal() { return m_normal; };
  inline glm::vec2 GetTexCoord() { return m_texCoord; };
  inline glm::vec3 GetTangent() { return m_tangent; };
  inline glm::vec3 GetBitangent() { return m_bitangent; };

  inline glm::vec3& GetPosRef() { return m_pos; };
  inline glm::vec3& GetNormalRef() { return m_normal; };
  inline glm::vec2& GetTexCoordRef() { return m_texCoord; };
  inline glm::vec3& GetTangentRef() { return m_tangent; };
  inline glm::vec3& GetBitangentRef() { return m_bitangent; };

private:
  glm::vec3 m_pos;
  glm::vec3 m_normal;
  glm::vec2 m_texCoord;
  glm::vec3 m_tangent;
  glm::vec3 m_bitangent;
};
#endif
