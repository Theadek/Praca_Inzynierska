#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Material.h"
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
public:

    std::vector<Vertex>     vertices;
    std::vector<GLuint>     indices;
    std::vector<Texture>    textures;
    GLuint VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    ~Mesh();

    void draw(Shader* shader);

private:
    GLuint VBO, EBO;

    void setupMesh();

};