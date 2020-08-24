#ifndef RENDERER_H
#define RENDERER_H
#include "ModelManager.h"
#include "ShaderManager.h"
class Renderer 
{
public:
    Renderer();
    void RenderModel(Model model, ShaderManager shader);
private:
    GLuint VAO;
};
#endif