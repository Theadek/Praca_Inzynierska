#ifndef RENDERER_H
#define RENDERER_H
#include "ModelManager.h"
#include "ShaderManager.h"
class Renderer 
{
public:
    Renderer();
    void RenderModel(Model model, ShaderManager shader, glm::vec3 position, glm::vec3 size, float rotate);
private:
    GLuint VAO;
};
#endif