#pragma once
#include<GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();   //ASSERT Function

/*NOTE : wrap all glfuntioncalls under GLCALL Macro*/
/*Automate way to error check each function*/
#define GLCALL(x) GLClearErrors();\
x;\
ASSERT(GLCallLog(#x,__FILE__,__LINE__))  
//#before turns any parameter in macro to string
//__FILE__ return current file path
//__LINE__ returns current line number

//clearing all errors
void GLClearErrors();

bool GLCallLog(const char* functionname, const char* filename, int errorline);

class Renderer
{
public:
	void Clear();
	void Draw(const VertexArray& VAO, const IndexBuffer& IBO, const Shader& shader);
};