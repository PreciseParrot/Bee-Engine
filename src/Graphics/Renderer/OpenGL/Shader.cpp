#include "Shader.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include <cstdlib>

#include "ErrorHandling.hpp"
#include "Bee/Log.hpp"

Shader::Shader() = default;

Shader::Shader(Shader&& other) noexcept
{
    rendererID = other.rendererID;
    other.rendererID = 0;

    uniformLocationCache = std::move(other.uniformLocationCache);
}

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
    createShader(vertexShader, fragmentShader);
}

void Shader::bind() const
{
    glCall(glUseProgram(rendererID));
}

void Shader::unbind() 
{
    glCall(glUseProgram(0));
}

void Shader::setUniform1f(const std::string& name, float data)
{
    glCall(glUniform1f(getUniformLocation(name), data));
}

void Shader::setUniform2f(const std::string& name, const Vector2f& data)
{
    glCall(glUniform2f(getUniformLocation(name), data.x, data.y));
}

void Shader::setUniform3f(const std::string& name, const Vector3f& data)
{
    glCall(glUniform3f(getUniformLocation(name), data.x, data.y, data.z));
}

void Shader::setUniform4f(const std::string &name, const Vector4f& data)
{
    glCall(glUniform4f(getUniformLocation(name), data.x, data.y, data.z, data.w));
}

void Shader::setUniformMat4f(const std::string& name, const Matrix4f& matrix)
{
    glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, (GLfloat*)&matrix));
}

unsigned int Shader::compileShader(const unsigned int type, const char* shaderSrc) 
{
    glCall(const unsigned int id = glCreateShader(type));
    glCall(glShaderSource(id, 1, &shaderSrc, nullptr));
    glCall(glCompileShader(id));

    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)calloc(length, sizeof(char));
        glCall(glGetShaderInfoLog(id, length, &length, message));
        //printf("Failed to compile shader: \n%s\n%s\n", shaderSrc, message);
        Log::write("Renderer", LogLevel::warning, "Failed to compile shader: \n%s\n", message);
        glCall(glDeleteShader(id));

        free(message);
        return 0;
    }

    return id;
}

void Shader::createShader(const char* vertexShader, const char* fragmentShader)
{
    glCall(rendererID = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(rendererID, vs));
    glCall(glAttachShader(rendererID, fs));
    glCall(glLinkProgram(rendererID));
    glCall(glValidateProgram(rendererID));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));
}

int Shader::getUniformLocation(const std::string& name)
{
    if (uniformLocationCache.contains(name))
    {
        return uniformLocationCache.at(name);
    }

    glCall(const int location = glGetUniformLocation(rendererID, name.c_str()));

    if (location == -1)
    {
        Log::write("Renderer", LogLevel::warning, "Uniform '%s' does not exist", name.c_str());
    }
    uniformLocationCache.insert({name, location});

    return location;
}

Shader::~Shader()
{
    glCall(glDeleteProgram(rendererID));
}