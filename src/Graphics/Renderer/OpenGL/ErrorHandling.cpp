#include "ErrorHandling.hpp"

#include "Bee/Log.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

void glClearError()
{
    while (glGetError());
}

bool glLogCall(const char* function)
{
    if (const GLenum error = glGetError())
    {
        Log::write("Renderer", LogLevel::error,"OpenGL Error: 0x%04x\n\tIn fucntion %s", error, function);
        return false;
    }
    return true;
}