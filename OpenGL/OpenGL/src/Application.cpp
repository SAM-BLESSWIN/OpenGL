#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Test/TestClearColor.h"

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

   { 
        /*Blending*/
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /*Renderer*/
        Renderer renderer;

        /*Set up Imgui*/

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        ImGui::StyleColorsDark();


        /*Test Framework*/
        test::TestClearColor test;

        //keep window open untill closed
        while (!glfwWindowShouldClose(window))
        {
            /*--Modern OpenGL--*/
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

            /*start imgui frame*/
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            test.OnImGuiRender();

            /*imgui render*/
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /*When an application draws in a single buffer the resulting image may display flickering issues.*/
            /*Double buffering : The front buffer contains the final output image that is shown at the screen,
            while all the rendering commands draw to the back buffer.*/
            // Swap front and back buffers 
            glfwSwapBuffers(window);

            //function checks if any events are triggered (like keyboard input or mouse movement events)
            glfwPollEvents();
        }
   }

   /*imgui shutdown*/
   ImGui_ImplOpenGL3_Shutdown();
   ImGui::DestroyContext();

    //Terminate GLFW
    glfwTerminate();
    return 0;
} 



