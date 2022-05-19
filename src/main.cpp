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
#include <array>
#include <algorithm>

#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

#define HEIGHT 720
#define WIDTH 1280

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = HEIGHT/2, lastY = WIDTH/2;
bool firstMouse = true;
float sensitivity = 0.075f;

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

struct Vertex {
    float Pos[3];
    float TexCoords[2];
    float TexId;
};

static Vertex* CreateCube(Vertex* target,float x, float y, float z) {
    
    target->Pos[0] = -0.5f + x;
    target->Pos[1] = -0.5f + y;
    target->Pos[2] = -0.5f + z;
    
    target->TexCoords[0] = 0.0f;
    target->TexCoords[1] = 0.0f;

    target->TexId = 0.0f;
    target++;
    
    target->Pos[0] =  0.5f + x;
    target->Pos[1] = -0.5f + y;
    target->Pos[2] = -0.5f + z;

    target->TexCoords[0] = 1.0f;
    target->TexCoords[1] = 0.0f;

    target->TexId = 0.0f;
    target++;
    
    target->Pos[0] =  0.5f + x;
    target->Pos[1] = -0.5f + y;
    target->Pos[2] =  0.5f + z;

    target->TexCoords[0] = 1.0f;
    target->TexCoords[1] = 1.0f;

    target->TexId = 0.0f;
    target++;
    
    target->Pos[0] = -0.5f + x;
    target->Pos[1] = -0.5f + y;
    target->Pos[2] =  0.5f + z;

    target->TexCoords[0] = 1.0f;
    target->TexCoords[1] = 0.0f;

    target->TexId = 0.0f;
    target++;

    target->Pos[0] = -0.5f + x;
    target->Pos[1] =  0.5f + y;
    target->Pos[2] = -0.5f + z;

    target->TexCoords[0] = 0.0f;
    target->TexCoords[1] = 1.0f;

    target->TexId = 0.0f;
    target++;

    target->Pos[0] =  0.5f + x;
    target->Pos[1] =  0.5f + y;
    target->Pos[2] = -0.5f + z;

    target->TexCoords[0] = 1.0f;
    target->TexCoords[1] = 1.0f;

    target->TexId = 0.0f;
    target++;

    target->Pos[0] =  0.5f + x;
    target->Pos[1] =  0.5f + y;
    target->Pos[2] =  0.5f + z;

    target->TexCoords[0] = 0.0f;
    target->TexCoords[1] = 1.0f;

    target->TexId = 0.0f;
    target++;
    
    target->Pos[0] = -0.5f + x;
    target->Pos[1] =  0.5f + y;
    target->Pos[2] =  0.5f + z;

    target->TexCoords[0] = 1.0f;
    target->TexCoords[1] = 1.0f;

    target->TexId = 0.0f;
    target++;

    return target;
}

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL: Cubes", NULL, NULL);
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
        -0.5f,-0.5f,-0.5f,    0.0f, 0.0f,   0.0f,
         0.5f,-0.5f,-0.5f,    1.0f, 0.0f,   0.0f,
         0.5f,-0.5f, 0.5f,    1.0f, 1.0f,   0.0f,
        -0.5f,-0.5f, 0.5f,    1.0f, 0.0f,   0.0f,
        
        -0.5f, 0.5f,-0.5f,    0.0f, 1.0f,   0.0f,
         0.5f, 0.5f,-0.5f,    1.0f, 1.0f,   0.0f,
         0.5f, 0.5f, 0.5f,    0.0f, 1.0f,   0.0f,
        -0.5f, 0.5f, 0.5f,    1.0f, 1.0f,   0.0f
    };*/

    /*
    uint32_t indices[] = { // 36 / 8v / 6s / 2t / 3600
        0, 3, 2,
        0, 2, 1,
        0, 1, 5,
        5, 4, 0,
        6, 1, 2,
        6, 5, 1,
        2, 7, 6,
        2, 3, 7,
        0, 4, 3,
        3, 4, 7,
        6, 7, 4,
        6, 4, 5,
        
    };*/

    uint32_t indices[3600];
    uint32_t indOffset = 0;
    for (int i = 0; i < 3600; i += 36)
    {
        indices[i + 0]  = 0 + indOffset;
        indices[i + 1]  = 3 + indOffset;
        indices[i + 2]  = 2 + indOffset;
        
        indices[i + 3]  = 0 + indOffset;
        indices[i + 4]  = 2 + indOffset;
        indices[i + 5]  = 1 + indOffset;
        
        indices[i + 6]  = 0 + indOffset;
        indices[i + 7]  = 1 + indOffset;
        indices[i + 8]  = 5 + indOffset;
        
        indices[i + 9]  = 0 + indOffset;
        indices[i + 10] = 5 + indOffset;
        indices[i + 11] = 4 + indOffset;
        
        indices[i + 12] = 6 + indOffset;
        indices[i + 13] = 1 + indOffset;
        indices[i + 14] = 2 + indOffset;
        
        indices[i + 15] = 6 + indOffset;
        indices[i + 16] = 5 + indOffset;
        indices[i + 17] = 1 + indOffset;
        
        indices[i + 18] = 2 + indOffset;
        indices[i + 19] = 7 + indOffset;
        indices[i + 20] = 6 + indOffset;
        
        indices[i + 21] = 2 + indOffset;
        indices[i + 22] = 3 + indOffset;
        indices[i + 23] = 7 + indOffset;
        
        indices[i + 24] = 0 + indOffset;
        indices[i + 25] = 4 + indOffset;
        indices[i + 26] = 3 + indOffset;
        
        indices[i + 27] = 3 + indOffset;
        indices[i + 28] = 4 + indOffset;
        indices[i + 29] = 7 + indOffset;
        
        indices[i + 30] = 6 + indOffset;
        indices[i + 31] = 4 + indOffset;
        indices[i + 32] = 5 + indOffset;
        
        indices[i + 33] = 6 + indOffset;
        indices[i + 34] = 7 + indOffset;
        indices[i + 35] = 4 + indOffset;
        
        indOffset += 8;
    }
    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(nullptr, 800 * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(1);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices)/sizeof(float));
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
    glm::vec3 camPos = glm::vec3(4.5f, 2.0f, 12.0f);
    glm::vec3 camFront = glm::vec3(0.0f,  0.0f, -1.0f);
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    ImVec4 clearColor = ImVec4(0.53f, 0.81f, 0.94f, 1.00f);
    float speed = 3.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        const int gridSize = 10;
        std::array<Vertex, gridSize*gridSize*8> vertices;

        Vertex* buffer = vertices.data();
        for (int j = 0; j < gridSize; j++)
        {
            for (int l = 0; l < gridSize; l++)
            {
                buffer = CreateCube(buffer, j, (j+l)%2, l);
            }
            
        }
        vb.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        
        
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
        /*
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[0]), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[1]), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot[2]), glm::vec3(0.0f, 0.0f, 1.0f));
        */

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            exit(0);
        } 
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        glfwSetCursorPosCallback(window, mouse_callback);

        if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
        {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

        float cameraSpeed = speed * deltaTime;
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            speed = 8.0f;
        } else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            speed = 1.0f;
        } else { speed = 2.5f; }

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
            /*
            ImGui::SliderFloat("Rot X", &rot[0], -180.0f, 180.0f);
            ImGui::SliderFloat("Rot Y", &rot[1], -180.0f, 180.0f);
            ImGui::SliderFloat("Rot Z", &rot[2], -180.0f, 180.0f);
            */
            ImGui::SliderFloat("Cam Speed", &speed, 0.1f, 10.0f);
            ImGui::SliderFloat("Cam Sens", &sensitivity, 0.01f, 1.0f);

            ImGui::ColorEdit3("Clear Color", (float*)&clearColor);

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
        glfwPollEvents();

    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
