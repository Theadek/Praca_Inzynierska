#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Material.h"
#include "ShaderManager.h"
class Mesh {
public:
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > texture_coordinates;
    std::vector< glm::vec3 > normals;
    Material  material;
    Mesh();
    void RenderMesh(GLuint VAO, ShaderManager shader, glm::vec3 position, glm::vec3 size, float rotate);
};