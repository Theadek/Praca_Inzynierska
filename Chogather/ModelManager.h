#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Material.h"
class Model
{
public:
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > texture_coordinates;
    std::vector< glm::vec3 > normals;
    std::vector< Material > materials;
    Model();
    bool loadobj(std::string path);
    void loadmtl(std::string path);
};

#endif