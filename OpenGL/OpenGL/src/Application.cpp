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
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    GLFWwindow* window = glfwCreateWindow(1920/2, 1080/2, "Lucifer Canvas", NULL, NULL);

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
        {   //Position    //Texture
             200.0f,100.0f,  0.0f,0.0f,   //0
             800.0f,100.0f,  1.0f,0.0f,   //1
             800.0f,400.0f,  1.0f,1.0f,   //2
             200.0f,400.0f,  0.0f,1.0f    //3
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        /*Vertex Buffer Object*/
        VertexBuffer VBO(vertices, 4 * 4 * sizeof(float));

        /*Vertex Array Object*/
        VertexArray VAO;

        VertexBufferLayout layout;
        layout.Push<float>(2);  //position attribute
        layout.Push<float>(2);  //texture attribute

        VAO.AddBuffer(VBO, layout);

        /*Element Buffer Object*/
        IndexBuffer IBO(indices, 6);

        /*Shader*/
        Shader shader("resources/shaders/Basic.shader");
         
        /*Texture*/
        Texture texture("resources/texture/godofwar.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0); //set uniform in shader to access texture in slot 0

        /*Blending*/
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VBO.Unbind();
        IBO.Unbind();
        VAO.Unbind();
        shader.Unbind();

        /*Projection*/
        glm::mat4 projection = glm::ortho(0.0f, (1920.0f / 2.0f), 0.0f, (1080.0f/2.0f),-1.0f,1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, 0.0f)); //simulating camera 
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,200.0f,0.0f));


        shader.Bind();
        shader.SetUniformMat4("u_Projection", projection);
        shader.SetUniformMat4("u_View", view);
        shader.SetUniformMat4("u_Model", model);

        Renderer renderer;

        float  r = 0.0f;
        float increment = 0.005f;

        //keep window open untill closed
        while (!glfwWindowShouldClose(window))
        {
            /*--Modern OpenGL--*/

            renderer.Clear();

            //animating color transition
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);
            if (r > 1.0f)
                increment = -0.005f;
            else if (r < 0)
                increment = 0.005f;
            r += increment;

            //Drawing 
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



