#include "Object.h"

void Object::renderModel(Shader* shader) {
    shader->use();
    glBindVertexArray(model->VAO);
    for (int i = 0; i < model->meshes.size(); i++) {
        model->meshes[i]->RenderMesh(shader, position, scale, rotate);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}