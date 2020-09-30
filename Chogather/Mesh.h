#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Material.h"
#include "Shader.h"
class Mesh {
public:
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > texture_coordinates;
    std::vector< glm::vec3 > normals;
    GLuint VBO;
    Material  material;
    Mesh();
    ~Mesh();
    void RenderMesh(Shader *shader, glm::vec3 position, glm::vec3 size, float rotate);
    void fillVBO();
};