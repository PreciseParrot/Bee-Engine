#pragma once

#include <string>
#include <unordered_map>

#include "Bee/Math/Matrix4f.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Bee/Math/Vector4f.hpp"

class Shader
{
public:
    Shader();
    Shader(Shader&& other) noexcept;
    Shader(const char* vertexShader, const char* fragmentShader);
    void createShader(const char* vertexShader, const char* fragmentShader);
    void bind() const;
    static void unbind();
    void setUniform1f(const std::string& name, float data);
    void setUniform2f(const std::string& name, const Vector2f& data);
    void setUniform3f(const std::string& name, const Vector3f& data);
    void setUniform4f(const std::string& name, const Vector4f& data);
    void setUniformMat4f(const std::string& name, const Matrix4f& matrix);
    ~Shader();

private:
    unsigned int rendererID = 0;
    std::unordered_map<std::string, int> uniformLocationCache;

    static unsigned int compileShader(unsigned int type, const char* shaderSrc) ;
    int getUniformLocation(const std::string& name);
};
