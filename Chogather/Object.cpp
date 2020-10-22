#include "Object.h"

void Object::renderModel(Shader* shader, float maxX, float maxY, glm::vec3 size) {
    shader->use();
    glBindVertexArray(model->VAO);
    for (int i = 0; i < model->meshes.size(); i++) {
        model->meshes[i]->RenderMesh(shader, position, scale, rotate, maxX, maxY, size);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}