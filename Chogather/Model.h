#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Material.h"
#include "Mesh.h"
#include "TextureManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    std::vector<Texture>    textures_loaded;
    std::vector<Mesh>       meshes;
    string directory;
    bool gammaCorrection;

    void draw(Shader* shader);

    Model(string const& path, bool gamma = false);
    ~Model();
private:

    void loadModel(string const &path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
