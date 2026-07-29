#include "Shader.h"

#include "../core/GLValidation.h"

Shader::Shader(std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = GlCall(glCreateShader(GL_VERTEX_SHADER));
    GlCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GlCall(glCompileShader(vertex));

    GlCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GlCall(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vertexPath << "\n" << infoLog << std::endl;
    };

    fragment = GlCall(glCreateShader(GL_FRAGMENT_SHADER));
    GlCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GlCall(glCompileShader(fragment));

    GlCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GlCall(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << fragmentPath << "\n" << infoLog << std::endl;
    };

    m_ID = GlCall(glCreateProgram());
    GlCall(glAttachShader(m_ID, vertex));
    GlCall(glAttachShader(m_ID, fragment));
    GlCall(glLinkProgram(m_ID));
    
    GlCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GlCall(glGetProgramInfoLog(m_ID, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    GlCall(glDeleteShader(vertex));
    GlCall(glDeleteShader(fragment));
}

Shader::Shader(std::filesystem::path geometryPath, std::filesystem::path vertexPath, std::filesystem::path fragmentPath) {
    std::string geometryCode;
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream gShaderFile;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        gShaderFile.open(geometryPath);
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream gShaderStream, vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        gShaderStream << gShaderFile.rdbuf();
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        gShaderFile.close();
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        geometryCode = gShaderStream.str();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* gShaderCode = geometryCode.c_str();
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int geometry, vertex, fragment;
    int success;
    char infoLog[512];

    // geometry Shader
    geometry = GlCall(glCreateShader(GL_GEOMETRY_SHADER));
    GlCall(glShaderSource(geometry, 1, &gShaderCode, NULL));
    GlCall(glCompileShader(geometry));

    GlCall(glGetShaderiv(geometry, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GlCall(glGetShaderInfoLog(geometry, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED: " << geometryPath << "\n" << infoLog << std::endl;
    };

    // vertex Shader
    vertex = GlCall(glCreateShader(GL_VERTEX_SHADER));
    GlCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GlCall(glCompileShader(vertex));

    GlCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GlCall(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vertexPath << "\n" << infoLog << std::endl;
    };

    // fragment Shader
    fragment = GlCall(glCreateShader(GL_FRAGMENT_SHADER));
    GlCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GlCall(glCompileShader(fragment));

    GlCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if (!success)
    {
        GlCall(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << fragmentPath << "\n" << infoLog << std::endl;
    };

    m_ID = GlCall(glCreateProgram());
    GlCall(glAttachShader(m_ID, vertex));
    GlCall(glAttachShader(m_ID, fragment));
    GlCall(glAttachShader(m_ID, geometry));
    GlCall(glLinkProgram(m_ID));

    GlCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
    if (!success)
    {
        GlCall(glGetProgramInfoLog(m_ID, 512, NULL, infoLog));
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    GlCall(glDeleteShader(geometry));
    GlCall(glDeleteShader(vertex));
    GlCall(glDeleteShader(fragment));
}

Shader::~Shader() {
    GlCall(glDeleteProgram(m_ID));
}

void Shader::Use() {
    GlCall(glUseProgram(m_ID));
}

void Shader::SetBool(const std::string& name, bool value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniform1i(location, (int)value));
}

void Shader::SetInt(const std::string& name, i32 value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniform1i(location, value));
}

void Shader::SetFloat(const std::string& name, f32 value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniform1f(location, value));
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniform2fv(location, 1, glm::value_ptr(value)));
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniform3fv(location, 1, glm::value_ptr(value)));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const {
    int location = GlCall(glGetUniformLocation(m_ID, name.c_str()));
    GlCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}