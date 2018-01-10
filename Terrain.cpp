#include "Terrain.hpp"

Terrain::Terrain(std::string fileName, unsigned int maxHeight) {
  MAX_HEIGHT = maxHeight;
  LoadBMPData(fileName);
  SetMeshData(m_BMPData);

  if (!m_vertices.empty()) {
    m_terrainMeshPtr = new Mesh(m_vertices, m_indices);
  }
}

void Terrain::SetMeshData(BMPData BMPData) {
  if (BMPData.good)
    {
      m_width = BMPData.width;
      m_height = BMPData.height;
      m_vertices.resize(m_width * m_height);
      m_indices.resize((m_width-1) * (m_height-1) * 2 * 2 * 3); // 2 to make quad, 2 to make triangles, 3 indices per triangle.

      ComputePos();
      ComputeNormals();
      ComputeTexCoords();
      ComputeIndices();
    }
}

void Terrain::LoadBMPData(std::string fileName) {
  // open file
  FILE* file = fopen(fileName.c_str(), "rb");
  if (!file)
    {
      std::cout << "Image could not be opened\n" << std::endl;
      m_BMPData.good = false;
      return;
    }
  if (fread(m_BMPData.header, 1, 54, file) != 54) // first 54 bytes has to be read before rgb data begins
    {
      std::cout << "Not a correct BMP file\n" << std::endl;
      m_BMPData.good = false;
      return;
    }
  if (m_BMPData.header[0] != 'B' || m_BMPData.header[1] != 'M')
    {
      std::cout << "File does not begin with 'BM'" << std::endl;
      m_BMPData.good = false;
      return;
    }

  // read ints fromm the byte array
  m_BMPData.dataPos = *(int*)&(m_BMPData.header[0x0A]);		// looking at a hexeditor on pos a it should have the vale 0x36 = 54
  m_BMPData.imageSize = *(int*)&(m_BMPData.header[0x22]);
  m_BMPData.width = *(int*)&(m_BMPData.header[0x12]);
  m_BMPData.height = *(int*)&(m_BMPData.header[0x16]);

  // checks if file is not correctly formated
  if (m_BMPData.imageSize == 0)
    m_BMPData.imageSize = m_BMPData.height*m_BMPData.width * 3;

  if (m_BMPData.dataPos == 0)
    m_BMPData.dataPos = 54;

  m_BMPData.data = new unsigned char[m_BMPData.imageSize];

  fread(m_BMPData.data, 1, m_BMPData.imageSize, file);

  m_BMPData.good = true;

  //std::cout << "-- Height map loading --" << std::endl;
  //std::cout << "Name: " << fileName << std::endl;
  //std::cout << "Width: " << m_BMPData.width << std::endl;
  //std::cout << "Height: " << m_BMPData.height << std::endl;
  //std::cout << "The pos where image data begins: " << m_BMPData.dataPos << std::endl;
  //std::cout << "-- Height map loading complete --" << std::endl;

  fclose(file);
}

void Terrain::SetTextureData(std::string fileName, std::string type) {
  m_texturePath = fileName;
  m_textureType = type;
}

void Terrain::ComputeIndices() {
  int i = 0;
  for (GLuint h = 0; h < m_height - 1; h++)// needs to be height - 1 to avoid make triangle when the last in row is finished
    {
      for (GLuint w = 0; w < m_width - 1; w++) // needs to be width - 1 to avoid make triangle when the last in row is finished
	{
	  // indicies
	  GLuint offset = (h * m_width + w);
          // quad
          m_indices[i++] = offset;
          m_indices[i++] = offset + m_width;
          m_indices[i++] = offset + 1;

          m_indices[i++] = offset + m_width + 1;
          m_indices[i++] = offset + 1;
          m_indices[i++] = offset + m_width;
	 }
    }
}

void Terrain::ComputePos() {
  for (GLuint h = 0; h < m_height; h++) {
      for (GLuint w = 0; w < m_width; w++) {
    	  // only need one color value to calc height, multiply by 3
        GLfloat color = (GLfloat)m_BMPData.data[3 * (h * m_width + w)];
    	  // convert to 0-1 float and half range
        // GLfloat currentHeightPerPixel = MAX_HEIGHT *((color / 255.0f) - 0.5f);
        GLfloat currentHeightPerPixel = MAX_HEIGHT *((color / 255.0f));
        // set height
	      SetHeight(h, w, currentHeightPerPixel);
	    }
  }
}

void Terrain::ComputeNormals() {
  if (computedNormals) {
    return;
  }

  //Compute the rough version of the normals
  glm::vec3** normals2 = new glm::vec3*[m_height];
  for (GLuint i = 0; i < m_height; i++) {
    normals2[i] = new glm::vec3[m_width];
  }

  for (GLuint z = 0; z < m_height; z++) {
    for (GLuint x = 0; x < m_width; x++) {
      glm::vec3 sum(0.0f, 0.0f, 0.0f);

      glm::vec3 out;
      if (z > 0) {
	out = glm::vec3(0.0f, m_vertices[(z - 1)*m_width + x].GetPos().y - m_vertices[z*m_width + x].GetPos().y, -1.0f);
      }
      glm::vec3 in;
      if (z < m_height - 1) {

	in = glm::vec3(0.0f, m_vertices[(z + 1)*m_width + x].GetPos().y - m_vertices[z*m_width + x].GetPos().y, 1.0f);
      }
      glm::vec3 left;
      if (x > 0) {
	left = glm::vec3(-1.0f, m_vertices[z*m_width + x-1].GetPos().y - m_vertices[z*m_width + x].GetPos().y, 0.0f);
      }
      glm::vec3 right;
      if (x < m_width - 1) {
	right = glm::vec3(1.0f, m_vertices[z*m_width + x + 1].GetPos().y - m_vertices[z*m_width + x].GetPos().y, 0.0f);
      }

      if (x > 0 && z > 0) {
	sum += glm::normalize(glm::cross(out, left));
      }
      if (x > 0 && z < m_height - 1) {
	sum += glm::normalize(glm::cross(left, in));
      }
      if (x < m_width - 1 && z < m_height - 1) {
	sum += glm::normalize(glm::cross(in, right));
      }
      if (x < m_width - 1 && z > 0) {
	sum += glm::normalize(glm::cross(right, out));
      }

      normals2[z][x] = glm::normalize(sum);

    }
  }

  //Smooth out the normals
  const float FALLOUT_RATIO = 0.5f;
  for (GLuint z = 0; z < m_height; z++) {
    for (GLuint x = 0; x < m_width; x++) {
      glm::vec3 sum = normals2[z][x];

      if (x > 0) {
	sum += normals2[z][x - 1] * FALLOUT_RATIO;
      }
      if (x < m_width - 1) {
	sum += normals2[z][x + 1] * FALLOUT_RATIO;
      }
      if (z > 0) {
	sum += normals2[z - 1][x] * FALLOUT_RATIO;
      }
      if (z < m_height - 1) {
	sum += normals2[z + 1][x] * FALLOUT_RATIO;
      }
      if (glm::length(sum) == 0) {
	sum = glm::vec3(0.0f, 1.0f, 0.0f);
      }

      m_vertices[z*m_width + x].GetNormal() = glm::normalize(sum);
    }
  }

  for (GLuint i = 0; i < m_height; i++) {
    delete[] normals2[i];
  }
  delete[] normals2;

  computedNormals = true;
}

void Terrain::ComputeTexCoords() {
  for (GLuint h = 0; h < m_height; h++) {
      for (GLuint w = 0; w < m_width; w++) {
	glm::vec2 texCoord;
	texCoord.x = (GLfloat)w / ((GLfloat)m_width - 1);
	texCoord.y = (GLfloat)h / ((GLfloat)m_height - 1);
	m_vertices[h * m_width + w].SetTexCoord(texCoord);
      }
  }
}

void Terrain::SetHeight(GLuint zPos, GLuint xPos, GLfloat height) {
  glm::vec3 pos = glm::vec3((GLfloat)xPos, height, (GLfloat)zPos);
  m_vertices[zPos*m_width + xPos].SetPos(pos);
  computedNormals = false;
}

Texture Terrain::TextureFromFile(const char *path, std::string typeName) {
  //Generate texture ID and load texture data
  std::string fileName = std::string(path);
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width, height;

  unsigned char *image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  // Assign texture to ID
  if (typeName == "texture_diffuse") {
    Bind2DTextureTo(textureID, MESHDIFF_TEX);
  }
  else if (typeName == "texture_specular") {
    Bind2DTextureTo(textureID, MESHSPEC_TEX);
  }
  else if (typeName == "texture_normal") {
    Bind2DTextureTo(textureID, NORMAL_TEX);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Unbind and free
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);

  // Prep texture
  Texture texture;
  texture.id = textureID;
  texture.type = typeName;
  texture.path = fileName;

  return texture;
 }

GLfloat Terrain::GetHeight(GLfloat xPos, GLfloat zPos) {
  return m_vertices[(int)zPos*m_width + (int)xPos].GetPos().y;
}

ModelData& Terrain::GetModelData() {
  // Package data for display
  std::vector<std::vector<glm::vec3> > terrainPos;
  std::vector<GLuint> terrainVAO;
  std::vector<std::vector<GLuint> > terrainIndice;

  terrainVAO.push_back(m_terrainMeshPtr->GetVAO());
  terrainIndice.push_back(m_indices);
  terrainPos.push_back(m_terrainMeshPtr->GetPos());

  m_modelData.s_insideFrustum = true;
  m_modelData.s_mode = GL_TRIANGLES;
  m_modelData.s_VAOs = terrainVAO;
  m_modelData.s_meshIndices = terrainIndice;
  // std::vector<std::vector<Texture> > vec2DTex(0, std::vector<Texture>(0));
  m_modelData.s_meshTextures[0].push_back(TextureFromFile(m_texturePath.c_str(), m_textureType));
  m_modelData.s_meshPos = terrainPos;
  m_modelData.s_modelMat = glm::mat4(1.0f);

  return m_modelData;
}

Terrain::~Terrain() {
  delete m_terrainMeshPtr;
}
