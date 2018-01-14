#include "Terrain.hpp"

Terrain::Terrain(std::string filePath, GLuint maxHeight, std::string diffTexPath, std::string specTexPath, std::string normTexPath) {
  MAX_HEIGHT = maxHeight;
  LoadBMPData(filePath);
  SetMeshData(m_BMPData);

  if (diffTexPath != "unknown") {
    Texture diffTex;
    TextureFromFile(diffTexPath.c_str(), "texture_diffuse", diffTex);
    // std::cout << "diff texture from terrain      (id): " << diffTex.id << '\n';
    m_textures.push_back(diffTex); // adding diffuse texture to m_textures
  }

  if (specTexPath != "unknown") {
    Texture specTex;
    TextureFromFile(specTexPath.c_str(), "texture_specular", specTex);
    // std::cout << "spectex texture from terrain   (id): " << specTex.id << '\n';
    m_textures.push_back(specTex); // adding specular texture to m_textures
  }

  if (normTexPath != "unknown") {
    Texture normTex;
    TextureFromFile(normTexPath.c_str(), "texture_normalmap", normTex);
    // std::cout << "normalMap texture from terrain (id): " << normTex.id << '\n';
    m_textures.push_back(normTex); // adding normal texture
  }

  if (!m_vertices.empty()) {
    // std::cout << "size of textures vector:" << m_textures.size() << '\n';
    m_terrainMeshPtr = new Mesh(m_vertices, m_indices, m_textures);
    PrepModelData();
  }

}

void Terrain::SetMeshData(BMPData BMPData) {
  if (BMPData.good) {
      //std::cout << "TERRAIN SIZE: " << BMPData.width << "x" << BMPData.height << '\n';
      m_width = BMPData.width;
      m_height = BMPData.height;
      m_vertices.resize(m_width * m_height);
      m_indices.resize((m_width-1) * (m_height-1) * 2 * 2 * 3); // 2 to make quad, 2 to make triangles, 3 indices per triangle.

      ComputePos();
      ComputeNormals();
      ComputeTexCoords();
      ComputeIndices();
      ComputeTangents();
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

<<<<<<< HEAD
      //m_vertices[z*m_width + x].GetNormal() = glm::normalize(sum);
=======
>>>>>>> vertex almost done
      m_vertices[z*m_width + x].SetNormal(glm::normalize(sum));
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

void Terrain::ComputeTangents() {
  GLuint i = 0;
  int offset = 0;
  for (size_t h = 0; h < m_height - 1; h++) {
    for (size_t w = 0; w < m_width - 1; w++) {
      offset = h * m_width + w;
      // std::cout << offset << std::endl;
      // std::cout << offset + m_width<< std::endl;
      // std::cout << offset + 1 << std::endl;
      if (i < m_vertices.size()) {
      for (size_t k = 0; k < 3; k++) {
        ComputeTangentBasis(m_vertices[offset].GetPosRef(), m_vertices[offset + m_width].GetPosRef(), m_vertices[offset + 1].GetPosRef(),
                            m_vertices[offset].GetTexCoordRef(), m_vertices[offset + m_width].GetTexCoordRef(), m_vertices[offset + 1].GetTexCoordRef(),
                            m_vertices[i].GetNormalRef(), m_vertices[i].GetTangentRef(), m_vertices[i].GetBitangentRef() );
        i++;
      }
    }
    // Explaination: Note that edge and texEdge are from the same triangle
    //               edge is expressed in modelspace and tex in UVs
    //               tanget is alined with u-coordinate of texCoord
    //               bitanget is alned with v-coordinate of texCoord
    //
    //               edge1 and 2 can be written as a linear combination of texCoord
    //               and tanget and bitanget.
    //
    //               tanget, bitanget and normal can be seen as a basis and the edges can be expressed in that world
    //               The problem can be expressed in matrix form:
    //               | E1x E1y E1z |   | texEdge1.x texEdge1.y | * | Tx Ty Tz |
    //               | E2x E2y E2z | = | texEdge2.x texEdge2.y |   | Bx By Bz |

  }
}
  // fix the last vertex
  m_vertices[i- 1].SetTangent(m_vertices[i-2].GetTangent());
  m_vertices[i - 1].SetBitangent(m_vertices[i-2].GetTangent());
}

void Terrain::ComputeTangentBasis(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2,
		                              const glm::vec2& UV0, const glm::vec2& UV1, const glm::vec2& UV2,
		                              glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent) {
		glm::vec3 e0 = P1 - P0;
		glm::vec3 e1 = P2 - P0;
		normal = glm::cross(e0, e1);
		// using Eric Lengyel's approach with a few modifications
		// from Mathematics for 3D Game Programmming and Computer Graphics
		// want to be able to trasform a vector in Object Space to Tangent Space
		// such that the x-axis cooresponds to the 's' direction and the
		// y-axis corresponds to the 't' direction, and the z-axis corresponds
		// to <0,0,1>, straight up out of the texture map

		//let P = v1 - v0
		glm::vec3 P = P1 - P0;
		//let Q = v2 - v0
		glm::vec3 Q = P2 - P0;
		float s1 = UV1.x - UV0.x;
		float t1 = UV1.y - UV0.y;
		float s2 = UV2.x - UV0.x;
		float t2 = UV2.y - UV0.y;


		//we need to solve the equation
		// P = s1*T + t1*B
		// Q = s2*T + t2*B
		// for T and B


		//this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
		//|px,py,pz| = |s1,t1| * |Tx,Ty,Tz|
		//|qx,qy,qz|   |s2,t2|   |Bx,By,Bz|

		//multiplying both sides by the inverse of the s,t matrix gives
		//|Tx,Ty,Tz| = 1/(s1t2-s2t1) *  |t2,-t1| * |px,py,pz|
		//|Bx,By,Bz|                    |-s2,s1|   |qx,qy,qz|
		//solve this for the unormalized T and B to get from tangent to object space

		float tmp = 0.0f;
		if(fabsf(s1*t2 - s2*t1) <= 0.0001f)
		{
			tmp = 1.0f;
		}
		else
		{
			tmp = 1.0f/(s1*t2 - s2*t1 );
		}

		tangent.x = (t2*P.x - t1*Q.x);
		tangent.y = (t2*P.y - t1*Q.y);
		tangent.z = (t2*P.z - t1*Q.z);

		tangent = tangent*tmp;

		bitangent.x = (s1*Q.x - s2*P.x);
		bitangent.y = (s1*Q.y - s2*P.y);
		bitangent.z = (s1*Q.z - s2*P.z);

		bitangent = bitangent*tmp;

    glm::normalize(normal);
		glm::normalize(tangent);
		glm::normalize(bitangent);
	}

void Terrain::SetHeight(GLuint zPos, GLuint xPos, GLfloat height) {
  glm::vec3 pos = glm::vec3((GLfloat)xPos, height, (GLfloat)zPos);
  m_vertices[zPos*m_width + xPos].SetPos(pos);
  computedNormals = false;
}

GLfloat Terrain::GetHeight(GLfloat xPos, GLfloat zPos) {
  return m_vertices[(int)zPos*m_width + (int)xPos].GetPos().y;
}

void Terrain::PrepModelData() {
  // Package data for display
  std::vector<std::vector<glm::vec3> > terrainPos;
  std::vector<GLuint> terrainVAO;
  std::vector<std::vector<GLuint> > terrainIndice;
  std::vector<std::vector<Texture> > terrainTexture;

  terrainVAO.push_back(m_terrainMeshPtr->GetVAO());
  terrainIndice.push_back(m_indices);
  terrainPos.push_back(m_terrainMeshPtr->GetPos());
  terrainTexture.push_back(m_terrainMeshPtr->m_textures);

  m_modelData.s_insideFrustum = true;
  m_modelData.s_mode = GL_TRIANGLES;
  m_modelData.s_VAOs = terrainVAO;
  m_modelData.s_meshIndices = terrainIndice;
  m_modelData.s_meshTextures = terrainTexture;
  m_modelData.s_meshPos = terrainPos;
  m_modelData.s_modelMat = glm::mat4(1.0f);
}

void Terrain::TextureFromFile(const char *path, std::string typeName, Texture& texture) {
  //Generate texture ID and load texture data
  std::string fileName = std::string(path);
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width, height;

  unsigned char *image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  // Assign texture to ID

  //glBindTexture(GL_TEXTURE_2D, textureID);
  //TEST:
  if (typeName == "texture_diffuse") {
    Bind2DTextureTo(textureID, MESHDIFF_TEX);
  }
  else if (typeName == "texture_specular") {
    Bind2DTextureTo(textureID, MESHSPEC_TEX);
  }
  else if (typeName == "texture_normalmap") {
    Bind2DTextureTo(textureID, NORMALMAP_TEX);
  }
  //TEST;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Unbind and free
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(image);

  texture.id = textureID;
  texture.type = typeName;
  texture.path = aiString(fileName);
}

ModelData& Terrain::GetModelData() {
  return m_modelData;
}

Terrain::~Terrain() {
  delete m_terrainMeshPtr;
}
