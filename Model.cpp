#include "Model.hpp"
#include "Vertex.hpp"
#include "texturefunctions.hpp"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

GLint TextureFromFile(const char* path, std::string directory);

Model::Model() : Transform() {

}

Model::Model(std::string path) : Transform() {
  LoadModel(path.c_str());
}

void Model::LoadModel(std::string path) {
  // Read file via assimp
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  // Check for errors
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    return;
  }
  // Retrieve the directory path of filepath
  m_directory = path.substr(0, path.find_last_of('/'));
  // Process assimps root node recursively
  ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  // Process each mesh located at the current node
  for (GLuint i = 0 ; i < node->mNumMeshes; i++) {
    // scence contain all the data. Node is just organized containers.
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

    m_meshes.push_back(ProcessMesh(mesh, scene));
  }
  // After all meshes are processed process each child nodes
  for (GLuint i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}


Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  // Data containers to fill
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  for (GLuint i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector; // place holder because assimp has its own vec3 datatype

    // Positions
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.SetPos(vector);

    // Normals
    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.SetNormal(vector);

    // if mesh contains texCoords
    if(mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      // in assimp a vertex can have 8 different texture coordinates. Assume first is best
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.SetTexCoord(vec);
    } else {
      vertex.SetTexCoord(glm::vec2(0.0f, 0.0f));
    }

    // Store newly filled vertex
    vertices.push_back(vertex);
  }

  // Indices
  for (GLuint i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // get all indices from the triangle face and store them
    for (GLuint j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // if it mesh has materials
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // Convension for sampler names in shader is
    // texture_diffuseX where X is a number. Same goes for Specular and Normal

    // Diffuse maps
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // Diffuse maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  // debug
  std::cout << "Creating mesh with "<< vertices.size() << " verts." << "\n";

  return Mesh(vertices, indices, textures);
};

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName) {
  std::vector<Texture> textures;
  for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
      aiString str;
      material->GetTexture(type, i, &str);

      // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
      bool skip = false;

      for (GLuint j = 0; j < m_textures_loaded.size(); j++)
	{
          if(m_textures_loaded[j].path == str)
	    {
              textures.push_back(m_textures_loaded[j]);
              skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
              break;
	    }
	}

      if(!skip)
	{   // If texture hasn't been loaded already, load it
          Texture texture;
          texture.id = TextureFromFile(str.C_Str(), m_directory, typeName);
          texture.type = typeName;
          texture.path = str;
          textures.push_back(texture);

          m_textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}
    }

  return textures;
}

GLint Model::TextureFromFile(const char *path, std::string directory, std::string typeName) {
  //Generate texture ID and load texture data
  std::string filename = std::string(path);
  filename = directory + '/' + filename;
  GLuint textureID;
  glGenTextures(1, &textureID);
  int width, height;

  unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  // Assign texture to ID

  //glBindTexture(GL_TEXTURE_2D, textureID);
  //TEST:
  if (typeName == "texture_diffuse") {
    Bind2DTextureTo(textureID, MESHDIFF_TEX);
  }
  else if (typeName == "texture_specular") {
    Bind2DTextureTo(textureID, MESHSPEC_TEX);
  }
  //TEST;

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

  return textureID;
}

std::vector<GLuint> Model::GetVAOs() {
  for (GLuint i = 0; i < m_meshes.size(); i++) {
    m_vaos.push_back(m_meshes[i].GetVAO());
  }
  return m_vaos;
}

std::vector<GLuint> Model::GetVBOs() {
  for (GLuint i = 0; i < m_meshes.size(); i++) {
    m_vbos.push_back(m_meshes[i].GetVBO());
  }
  return m_vbos;
}

std::vector<GLuint> Model::GetEBOs() {
  for (GLuint i = 0; i < m_meshes.size(); i++) {
    m_ebos.push_back(m_meshes[i].GetEBO());
  }
  return m_ebos;
}

std::vector<std::vector<GLuint> > Model::GetModelMeshesIndices() {
  std::vector<std::vector<GLuint> > retMeshesIndices;
  for(GLuint i = 0; i < m_meshes.size(); i++) {
    retMeshesIndices.push_back(m_meshes[i].m_indices);
  }
  return retMeshesIndices;
}

std::vector<Mesh> Model::GetModelMeshes() { return m_meshes; }

std::vector<std::vector<glm::vec3> > Model::GetModelMeshesPos() {
  std::vector<std::vector<glm::vec3> > retMeshesPos;

  for(GLuint i = 0; i < m_meshes.size(); i++) {     // Loop through each mesh
    retMeshesPos.push_back(m_meshes[i].GetPos());
  }
  return retMeshesPos;
}

std::vector< std::vector<Texture> > Model::GetMeshTextures() {
  std::vector< std::vector<Texture> > retTextures;

  for (GLuint i = 0; i < m_meshes.size(); i++) {
    retTextures.push_back(m_meshes[i].m_textures);
  }

  return retTextures;
}

ModelData& Model::GetModelData() {
  m_modelData.s_insideFrustum = false;
  m_modelData.s_mode = GL_TRIANGLES;
  m_modelData.s_VAOs = GetVAOs();
  m_modelData.s_meshIndices = GetModelMeshesIndices();
  m_modelData.s_meshTextures = GetMeshTextures();
  m_modelData.s_modelMat = GetModelMatrix();

  return m_modelData;
}

Model::~Model() {}
