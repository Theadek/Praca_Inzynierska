#include "Mesh.h"
Mesh::Mesh() {

}

void Mesh::RenderMesh(GLuint VAO, ShaderManager shader, glm::vec3 position, glm::vec3 size, float rotate) {
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (this->vertices.size() * sizeof(glm::vec3) + this->texture_coordinates.size() * sizeof(glm::vec2)), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->texture_coordinates.size() * sizeof(glm::vec2), &this->texture_coordinates[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(this->vertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    shader.use();
    shader.setInt("diffuse_tex", 0);
    shader.setInt("specular_tex", 1);
    this->material.diffuseTexture.Bind();
    this->material.specularTexture.Bind();
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::scale(model_matrix, size);
    model_matrix = glm::translate(model_matrix, position);  
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 

    shader.setMat4("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    glDeleteBuffers(1, &VBO);
}