#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void DrawImmediate();

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "GPU Rendering", NULL, NULL);

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

    /*Shader Code*/
    std::string vertexShader =
        "#version 330 core\n"     //use opengl version 330
        "\n"
        "layout(location = 0) in vec4 position;\n"  //location=0 is the index of the attribute
        "void main()\n"
        "{\n"
        " gl_Position = position;\n"
        "}\n";

    std::string fragmentShader=
        "#version 330 core\n"     //use opengl version 330
        "\n"
        "layout(location = 0) out vec4 color;\n"  //location=0 is the index of the attribute
        "void main()\n"
        "{\n"
        " color=vec4(1.0,0.0,0.0,1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
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

    glDeleteShader(shader);

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