#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Material.h"
#include "Mesh.h"
#include "Object.h"
class Model
{
public:
    std::vector< Material > materials;
    std::vector< Mesh *> meshes;
    GLuint VAO;
    Model();
    ~Model();
    bool loadobj(std::string path);
    bool loadmtl(std::string path);
    void renderModel(ShaderManager* shader, Object* object);
};

#endif