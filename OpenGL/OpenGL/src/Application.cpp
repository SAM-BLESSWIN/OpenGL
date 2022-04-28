#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void DrawImmediate();

int main(void)
{
    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    /*State setting function of GLFW has GLFW_ prefix*/
    
    // OpenGL version 3.3 
    //setting state context version to 3 and above
    //This ensures that when a user does not have the proper OpenGL version GLFW fails to run. 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    //setting opengl rendering profile
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //compat profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //core profile

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

    /*Vertex Buffer Data*/
    unsigned int VBO;
    glGenBuffers(1, &VBO); //bufferid contains the id of that buffer
    //opengl keeps using the binded id untill someother id is binded or this is unbinded
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //selecting that id and setting to interpret as a array 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices,GL_STATIC_DRAW); //allocating and initializing data

    glEnableVertexAttribArray(0); //enabling vertex atribute by passing its starting index
    
    /*Vertex Attributes*/
    //index - starting index of the vertex
    //size - count of data in each vertex
    //type - datatype of the attribute
    //normalize? - whether we want gpu to normalize data or not 
    //stride - amount of bytes between each vertex 
    //pointer - offset of each attribute in bytes
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(const void*)0); //posititon attribute


    //keep window open untill closed
    while (!glfwWindowShouldClose(window))
    {
        //clearing the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        /*--Immediate mode--*/
        //DrawImmediate();

        /*When an application draws in a single buffer the resulting image may display flickering issues.*/
        /*Double buffering : The front buffer contains the final output image that is shown at the screen, 
        while all the rendering commands draw to the back buffer.*/
        // Swap front and back buffers 
        glfwSwapBuffers(window);

        //function checks if any events are triggered (like keyboard input or mouse movement events)
        glfwPollEvents();
    }

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