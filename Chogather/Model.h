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
class Model
{
public:
    std::vector< Material > materials;
    std::vector< Mesh *> meshes;
    GLuint VAO;
    std::string name;
    glm::vec3 size, corner;
    Model();
    ~Model();
    bool loadobj(std::string path);
    bool loadmtl(std::string path);
};
