#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void DrawImmediate(); //Immediate Mode

//Used for returning multiple object 
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


static ShaderProgramSource ParseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    ShaderType type=ShaderType::NONE;

    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2]; 

    while (std::getline(stream,line))
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
            if(type!=ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }

    }

    return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //(char* src = &source[0]) address of 1st char in string

    //id - shader id
    //count - number of source code passing
    //address of the array(char*) holding pointers to all source code
    //array containing length of each string
    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);

    /*Debugging Error*/
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) //GL_FALSE =0 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
       // std::unique_ptr<char[]> message = std::make_unique<char[]>(length * sizeof(char));  //dynamic allocation in heap
        char* message = (char*)alloca(length * sizeof(char)); //dynamic allocation in stack
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " 
            <<(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader!!!"<<std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }
    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int programId = glCreateProgram(); //return an id

    /*Compiling Shader*/
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    /*Attach both shader together to same program*/
    glAttachShader(programId,vs);
    glAttachShader(programId,fs);

    /*Links the shaders attached to program*/
    glLinkProgram(programId);

    /*Validate the program*/
    glValidateProgram(programId);

    /*Deleting the shader code because it is attached to program and validated*/
    glDeleteShader(vs);
    glDeleteShader(fs);

    return programId;
}

int main(void)
{
    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    /*State setting function of GLFW has GLFW_ prefix*/
    
    //creating a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lucifer Canvas", NULL, NULL);

    //Error check
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //setting all the context for current window
    glfwMakeContextCurrent(window);

    //Rendering area
    glViewport(0, 0, 800, 600);

    /* Initialize GLEW */  //GLEW must be initialized only after creating a valid opengl context
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Initialize Error!!!" << std::endl;

    /*Current OpenGL version*/
    std::cout << glGetString(GL_VERSION) << std::endl;

    //vertex doesn't contain only position 
    //it also include various attributes such as position,texture co-ord,colors,normals,etc...
    float vertices[6] =
    {
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f,-0.5f
    };

    /*Vertex Buffer Object*/
    unsigned int VBO;
    glGenBuffers(1, &VBO); //bufferid contains the id of that buffer
    //opengl keeps using the binded id untill someother id is binded or this is unbinded
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //selecting that id and setting to interpret as a array 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices,GL_STATIC_DRAW); //allocating and initializing data

    glEnableVertexAttribArray(0); //enabling vertex atribute by passing its index
    
    /*Vertex Attributes*/
    //index - index of the attribute
    //size - count of data in each vertex
    //type - datatype of the attribute
    //normalize? - whether we want gpu to normalize data or not 
    //stride - amount of bytes between each vertex 
    //pointer - offset of each attribute in bytes
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(const void*)0); //posititon attribute

    /*Parsing a shader from file*/
    ShaderProgramSource shadersource = ParseShader("resources/shaders/Basic.shader");
    std::cout << "Vertex Shader" << std::endl;
    std::cout << shadersource.VertexSource<<std::endl;
    std::cout << "Fragment Shader" << std::endl;
    std::cout << shadersource.FragmentSource<<std::endl;

    /*Creating and Compiling shader program*/
    unsigned int shader = CreateShader(shadersource.VertexSource, shadersource.FragmentSource);
    glUseProgram(shader); //Binding to use the specified shader

    //keep window open untill closed
    while (!glfwWindowShouldClose(window))
    {
        //clearing the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        /*--Immediate mode--*/
        //DrawImmediate();

        /*--Modern OpenGL--*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /*When an application draws in a single buffer the resulting image may display flickering issues.*/
        /*Double buffering : The front buffer contains the final output image that is shown at the screen, 
        while all the rendering commands draw to the back buffer.*/
        // Swap front and back buffers 
        glfwSwapBuffers(window);

        //function checks if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    //Terminate GLFW
    glfwTerminate();
    return 0;
} 

void DrawImmediate()
{
#if 0
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex2f(-0.5f, -0.5f);

    glColor3f(0, 1, 0);
    glVertex2f(0, 0.5f);

    glColor3f(0, 0, 1);
    glVertex2f(0.5f, -0.5f);
    glEnd();
#endif

#if 0
    //without depth
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); //red
    glVertex2f(-0.5f, -0.5);
    glVertex2f(0, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0); //green
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glEnd();
#endif

#if 1
    //with depth
    glEnable(GL_DEPTH_TEST);

    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); //red
    glVertex3f(-0.5f, -0.5, 0.1f);
    glVertex3f(0, 0.5f, 0.1f);
    glVertex3f(0.5f, -0.5f, 0.1f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0, 1, 0); //green
    glVertex3f(-0.5f, 0.5f, 0.2f);
    glVertex3f(0, -0.5f, 0.2f);
    glVertex3f(0.5f, 0.5f, 0.2f);
    glEnd();
#endif
}