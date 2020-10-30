#include "debugDrawer.h"

debugDrawer::debugDrawer() :m_debugMode(0) {
}


debugDrawer::debugDrawer(Camera* camera) : m_debugMode(0) {
    this->camera = camera;
    this->shader = new Shader("Shaders/collisionBoxes.vert", "Shaders/collisionBoxes.frag");
}

void debugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    GLfloat vertices[]{
            from.getX(), from.getY(), from.getZ(),
            to.getX(), to.getY(), to.getZ()
    };
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    GLuint lineVAO, lineVBO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    shader->use();
    shader->setMat4("model", glm::mat4(1.0f));
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    glBindVertexArray(lineVAO);
    glLineWidth(4.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1.0f);
    glDeleteVertexArrays(1, &lineVAO);
    glDeleteBuffers(1, &lineVBO);
}
void  debugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {

}

void debugDrawer::reportErrorWarning(const char* warningString) {
    printf(warningString);
}

void debugDrawer::draw3dText(const btVector3& location, const char* textString) {

}

void debugDrawer::setDebugMode(int debugMode) {
    m_debugMode = debugMode;
}

int debugDrawer::getDebugMode() const {
    return 1;
}
