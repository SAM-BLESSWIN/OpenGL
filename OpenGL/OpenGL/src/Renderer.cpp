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

void Renderer::Clear()
{
    //clearing the screen
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader)
{
    //draw using vertices
    //glDrawArrays(GL_TRIANGLES, 0, 3); 

    /* Manual way to error check each function
    GLClearErrors();
    //draw using indices
    glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); //indices should always be unsigned type
    ASSERT(GLCallLog());  */

    VAO.Bind();
    IBO.Bind();
    shader.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
}
