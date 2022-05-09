#include "Renderer.h"
#include<iostream>

void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR); //GL_NO_ERROR = 0
}

bool GLCallLog(const char* functionname, const char* filename, int errorline)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR] (" << error << ") : " << functionname << " " << filename << " : " << errorline << std::endl;
        return false;
    }
    return true;
}