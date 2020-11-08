#pragma once
#include "UIElement.h"
#include "Font.h"
#include "glm/glm.hpp"
#include <string>


class UIText : public UIElement {
public:
    UIText(float x, float y, float scale, std::string text, Font* font, glm::vec3 color) : UIElement(x,y){
        this->scale = scale;
        this->text = text;
        this->font = font;
        this->color = color;
    }
    ~UIText() {}
    std::string text;
    Font* font;
    float scale;
    glm::vec3 color;

    void draw(Shader* shader) {
        font->RenderText(shader, text, x, y, scale, color);
    }
};