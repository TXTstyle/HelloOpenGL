#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

//#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

//#include "IndexBuffer.hpp"
//#include "VertexArray.hpp"
//#include "Shader.hpp"



int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(680, 680, "Hello OpenGL: Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        throw std::runtime_error("GLEW not OK!");


    std::cout << glGetString(GL_VERSION) << std::endl;

    float pos[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(pos, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    IndexBuffer ib(indices, 6);
    
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.5, 0.15, 0.7, 1.0);

    Texture texture("res/textures/Grass_texture2.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    float red = 0.0f;
    float add = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(errorOccurredGL, NULL);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.SetUniform4f("u_Color", red, 0.15, 0.7, 1.0);

        renderer.Draw(va, ib, shader);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (red > 1.0f)
        {
            add = -0.01f;
        } else if(red < 0.0f) {
            add = 0.01f;
        }
        
        red += add;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}