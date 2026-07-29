#pragma once
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/Types.h"

class Shader {
public:
    Shader() {}
    Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    Shader(std::filesystem::path geometryPath, std::filesystem::path vertexPath, std::filesystem::path fragmentPath);
    ~Shader();

    void Use();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, i32 value) const;
    void SetFloat(const std::string& name, f32 value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;
private:
    unsigned int m_ID;
};

