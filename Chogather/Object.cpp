#include "Object.h"

void Object::renderModel(Shader* shader) {
    shader->use();
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
    shader->setMat4("model", modelMatrix);
    for (int i = 0; i < model->meshes.size(); i++) {
        model->meshes[i].draw(shader);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}