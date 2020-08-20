#include "Renderer.h"
#include <glm/ext.hpp>
#include "TextureManager.h"
using namespace std;
Renderer::Renderer() {
    //glGenVertexArrays(1, &this->VAO);
    //glBindVertexArray(this->VAO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glEnableVertexAttribArray(0);
}

void Renderer::RenderModel(Model model, ShaderManager shader, glm::vec3 position, glm::vec3 size, float rotate) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(glm::vec3), &model.vertices[0], GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, (model.vertices.size() * sizeof(glm::vec3) + model.texture_coordinates.size() * sizeof(glm::vec2)), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, model.vertices.size() * sizeof(glm::vec3), &model.vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(glm::vec3), model.texture_coordinates.size() * sizeof(glm::vec2), &model.texture_coordinates[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(model.vertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    shader.use();
    shader.setInt("diffuse_tex", 0);
    shader.setInt("specular_tex", 1);
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::scale(model_matrix, glm::vec3(size));
    model_matrix = glm::translate(model_matrix, position);  
    model_matrix = glm::translate(model_matrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z)); 
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model_matrix = glm::translate(model_matrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.5f * size.z)); 

    shader.setMat4("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());
}

