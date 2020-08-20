#pragma once
#include <glm/ext.hpp>
#include <string>
#include "TextureManager.h"
class Material {
public:
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Texture2D diffuseTexture;
    Texture2D specularTexture;
    Material() {

    }
    Material(std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Texture2D diffuseTexture, Texture2D specularTexture) {
        this->name = name;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->diffuseTexture = diffuseTexture;
        this->specularTexture = specularTexture;
    }

};