#pragma once
#include "Shader.h"


class UIElement {
public:
    UIElement(float x, float y) : x(x), y(y)
    {}

    ~UIElement() {}
    float x, y;

    virtual void draw(Shader* shader) = 0;
};