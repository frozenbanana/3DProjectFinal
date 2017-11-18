#ifndef VERTEX_H
#define VERTEX_H
#include <glm/glm.hpp>

class Vertex
{
 public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0.f, 0.f, 0.f))
    {
      m_pos = pos;
      m_texCoord = texCoord;
      m_normal = normal;
    }

	inline glm::vec3* GetPos() { return &m_pos; };
	inline glm::vec2* GetTexCoord() { return &m_texCoord; };
	inline glm::vec3* GetNormal() { return &m_normal; };

 private:
	glm::vec3 m_pos;
	glm::vec2 m_texCoord;
	glm::vec3 m_normal;
};
#endif
