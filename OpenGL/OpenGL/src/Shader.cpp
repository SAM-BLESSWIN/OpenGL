#include "Shader.h"
#include "Renderer.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath):m_Filepath(filepath),m_RendererID(0)
{
    /*Parsing a shader from file*/
    ShaderProgramSource shadersource = ParseShader(filepath);

    //std::cout << "Vertex Shader" << std::endl;
    //std::cout << shadersource.VertexSource<<std::endl;
    //std::cout << "Fragment Shader" << std::endl;
    //std::cout << shadersource.FragmentSource<<std::endl; 

    /*Creating and Compiling shader program*/
    m_RendererID = CreateShader(shadersource.VertexSource, shadersource.FragmentSource);
    GLCALL(glUseProgram(m_RendererID)); //Binding to use the specified shader 
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }

    }

    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char* src = source.c_str(); //(char* src = &source[0]) address of 1st char in string

    //id - shader id
    //count - number of source code passing
    //address of the array(char*) holding pointers to all source code
    //array containing length of each string
    GLCALL(glShaderSource(id, 1, &src, nullptr));

    GLCALL(glCompileShader(id));

    /*Debugging Shader Errors*/
    int result;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) //GL_FALSE =0 
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // std::unique_ptr<char[]> message = std::make_unique<char[]>(length * sizeof(char));  //dynamic allocation in heap
        char* message = (char*)alloca(length * sizeof(char)); //dynamic allocation in stack
        GLCALL(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader!!!" << std::endl;
        std::cout << message << std::endl;

        GLCALL(glDeleteShader(id));
        return 0;
    }
    return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCALL(unsigned int programId = glCreateProgram()); //return an id

    /*Compiling Shader*/
    GLCALL(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCALL(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    /*Attach both shader together to same program*/
    GLCALL(glAttachShader(programId, vs));
    GLCALL(glAttachShader(programId, fs));

    /*Links the shaders attached to program*/
    GLCALL(glLinkProgram(programId));

    /*Validate the program*/
    GLCALL(glValidateProgram(programId));

    /*Deleting the shader code because it is attached to program and validated*/
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return programId;
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID)); //Binding to use the specified shader 
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0)); 
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCALL(glUniform1i(GetUniformLocation(name), value)); //1i - 1 parameter of int
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCALL(glUniform1f(GetUniformLocation(name), value)); //1f - 1 parameter of float
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); //4f - 4 parameters of float 
}

unsigned int Shader::GetUniformLocation(const std::string& name) 
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning : uniform ' " << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
	return location;
}
