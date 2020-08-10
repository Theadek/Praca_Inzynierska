#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <vector>
#include <glm/glm.hpp>
class Model
{
public:
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > texture_coordinates;
    std::vector< glm::vec3 > normals;
    Model();
    bool loadobj(const char *path);
};

#endif