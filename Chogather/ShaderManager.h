#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderManager
{
public:
    unsigned int ID;

    ShaderManager(const char* vertexPath, const char* fragmentPath);
    ~ShaderManager();
    void use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec3) const;
};

