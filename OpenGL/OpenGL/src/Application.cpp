#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    /* Initialize GLFW */
    if (!glfwInit())
        return -1;

    /*State setting function of GLFW has GLFW_ prefix*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    
    glfwSwapInterval(1); //vsync //want to sync your rendering loop to the refresh rate of the monitor 

    /* Initialize GLEW */  //GLEW must be initialized only after creating a valid opengl context
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Initialize Error!!!" << std::endl;

    /*Current OpenGL version*/
    std::cout << glGetString(GL_VERSION) << std::endl;

   { //this scope is needed to delete stack allocated objects[VBO,IBO] to get deleted before glfw window terminates
        //vertex doesn't contain only position 
        //it also include various attributes such as position,texture co-ord,colors,normals,etc...
        float vertices[] =
        {
            -0.5f,-0.5f,  //0
             0.5f,-0.5f,  //1
             0.5f, 0.5f,  //2
            -0.5f, 0.5f   //3
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        /*Vertex Buffer Object*/
        VertexBuffer VBO(vertices, 4 * 2 * sizeof(float));

        /*Vertex Array Object*/
        VertexArray VAO;

        VertexBufferLayout layout;
        layout.Push<float>(2);  //attribute

        VAO.AddBuffer(VBO, layout);

        /*Element Buffer Object*/
        IndexBuffer IBO(indices, 6);

        /*Shader*/
        Shader shader("resources/shaders/Basic.shader");

        VBO.Unbind();
        IBO.Unbind();
        VAO.Unbind();
        shader.Unbind();

        Renderer renderer;

        float  r = 0.0f;
        float increment = 0.05f;

        //keep window open untill closed
        while (!glfwWindowShouldClose(window))
        {
            /*--Modern OpenGL--*/

            renderer.Clear();

            //draw using vertices
            //glDrawArrays(GL_TRIANGLES, 0, 3); 


            /* Manual way to error check each function
            GLClearErrors();
            //draw using indices
            glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); //indices should always be unsigned type
            ASSERT(GLCallLog());  */


            //animating color transition
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;

            r += increment;

            renderer.Draw(VAO,IBO,shader);
            

            /*When an application draws in a single buffer the resulting image may display flickering issues.*/
            /*Double buffering : The front buffer contains the final output image that is shown at the screen,
            while all the rendering commands draw to the back buffer.*/
            // Swap front and back buffers 
            glfwSwapBuffers(window);

            //function checks if any events are triggered (like keyboard input or mouse movement events)
            glfwPollEvents();
        }
   }
    //Terminate GLFW
    glfwTerminate();
    return 0;
} 



