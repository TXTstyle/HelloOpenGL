#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

#define HEIGHT 640
#define WIDTH 960

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = HEIGHT/2, lastY = WIDTH/2;
bool firstMouse = true;
float sensitivity = 0.1f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;  

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
}

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL: Triangle", NULL, NULL);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(330));
    /*
    float pos[] = {
        -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.0f,-0.5f, 1.0f, 0.0f,
         0.5f, 0.0f,-0.5f, 0.0f, 0.0f,
         0.5f, 0.0f, 0.5f, 1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.5f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };
    */

    float pos[] = {
        -0.5f, 0.0f, 0.5f,    0.0f, 0.0f,
        -0.5f, 0.0f,-0.5f,    1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,    1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,    1.0f, 0.0f,
        
        -0.5f, 1.0f, 0.5f,    0.0f, 1.0f,
        -0.5f, 1.0f,-0.5f,    1.0f, 1.0f,
         0.5f, 1.0f,-0.5f,    0.0f, 1.0f,
         0.5f, 1.0f, 0.5f,    1.0f, 1.0f,
    };
    
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 5,
        0, 4, 5,
        1, 2, 5,
        2, 5, 6,
        2, 3, 6,
        3, 6, 7,
        0, 3, 4,
        3, 4, 7,
        4, 5, 6,
        4, 6, 7
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(pos, 8 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices)/sizeof(float));
    /*
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    */
    glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    Texture texture("res/textures/Grass_texture.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;
    glm::vec3 camPos = glm::vec3(0.0f, 1.0f, 5.0f);
    glm::vec3 camFront = glm::vec3(0.0f,  0.0f, -1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float rot[] = { 0, 0, 0 };
    ImVec4 clearColor = ImVec4(0.53f, 0.81f, 0.94f, 1.00f);
    float speed = 3.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(errorOccurredGL, NULL);
        /* Render here */
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rot[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[2]), glm::vec3(0.0f, 0.0f, 1.0f));

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
            glfwSetCursorPosCallback(window, mouse_callback);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
            glfwSetCursorPosCallback(window, NULL);
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camFront = glm::normalize(direction);

        view = glm::lookAt(camPos, camPos + camFront, camUp);
        proj = glm::perspective(glm::radians(45.0f), ((float)WIDTH/HEIGHT), 0.1f, 100.0f);

        glm::mat4 mvp = proj * view * model;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
        
        {
            float cameraSpeed = speed * deltaTime;
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                camPos += cameraSpeed * camFront;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                camPos -= cameraSpeed * camFront;
            }
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                camPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                camPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
            }
        }

        {
            ImGui::Begin("Hello, world!");

            ImGui::SliderFloat("Rot X", &rot[0], -180.0f, 180.0f);
            ImGui::SliderFloat("Rot Y", &rot[1], -180.0f, 180.0f);
            ImGui::SliderFloat("Rot Z", &rot[2], -180.0f, 180.0f);
            
            ImGui::SliderFloat("Cam Speed", &speed, 0.1f, 10.0f);
            ImGui::SliderFloat("Cam Sens", &sensitivity, 0.01f, 1.0f);

            ImGui::ColorEdit3("clear color", (float*)&clearColor);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwWaitEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}