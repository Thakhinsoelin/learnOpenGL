#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include "shaderLoader.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureS {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureS>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureS> textures);
    void Draw(shaderLoader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

class Model
{
public:
    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(shaderLoader& shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
	std::vector<TextureS> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureS> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};

