#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Material.h"
#include "Mesh.h"
class Model
{
public:
    std::vector< Material > materials;
    std::vector< Mesh *> meshes;
    glm::vec3 position;
    glm::vec3 size;
    GLfloat rotate;
    GLuint VAO;
    Model();
    ~Model();
    bool loadobj(std::string path);
    void loadmtl(std::string path);
    void renderModel(ShaderManager* shader);
};

#endif