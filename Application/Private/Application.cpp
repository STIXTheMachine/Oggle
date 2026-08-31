#include "Application/Application.hpp"
#include "LogApplication.hpp"
#include "RenderingModule.hpp"
#include "WindowingModule.hpp"
#include "Renderer/Renderer.hpp"
#include "glad/gl.h" // TODO: figure out why I can include this here
#include "GLFW/glfw3.h"

namespace Oggle {
void Application::Init(const ApplicationInitInfo& AppInfo)
{
    WindowCreateInfo WindowInfo
    {
        .Name = AppInfo.Name
    };

    WindowingModule::Init();

    CreateMainWindow(WindowInfo);
    MainWindow->MakeContextCurrent();

    RenderingModule::Init();

    RendererCreateInfo RendererInfo {

    };


    CreateRenderer(RendererInfo);
}

void Application::Run()
{
    uint32 VAO, VBO, EBO;

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);

    struct Vertex { float X; float Y; float Z; };

    const Vertex Triangle[3]
    {
        { -0.5f, -0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        {  0.0f,  0.5f, 0.0f }
    };

    // Buffer the data
    glNamedBufferData(VBO, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
    // Attach VBO to VAO binding point 0
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
    // Tell OpenGL how to interpret the data at attribute 0
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    // Tell OpenGL to get data for attribute 0 from the buffer at binding point 0
    glVertexArrayAttribBinding(VAO, 0, 0);
    // Enable attribute 0
    glEnableVertexArrayAttrib(VAO, 0);

    const uint32 Indices[3] { 0, 1, 2 };
    glNamedBufferData(EBO, sizeof(Indices), Indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(VAO, EBO);

    const char* VertexSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

    const char* FragmentSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f)\n"
    "}\n";

    uint32 VertexShader   = glCreateShader(GL_VERTEX_SHADER);
    uint32 FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    uint32 ShaderProgram  = glCreateProgram();

    glShaderSource(VertexShader, 1, &VertexSource, 0);
    glShaderSource(FragmentShader,1, &FragmentSource, 0);

    int Success;

    glCompileShader(VertexShader);
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        char InfoLog[512];
        glGetShaderInfoLog(VertexShader, 512, nullptr, InfoLog);
        LOG(Application, FMT("Failed to compile vertex shader:\n{}", InfoLog));
    }

    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        char InfoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, nullptr, InfoLog);
        LOG(Application, FMT("Failed to compile fragment shader:\n{}", InfoLog));
    }

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        char InfoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, InfoLog);
        LOG(Application, FMT("Failed to link shader program:\n{}", InfoLog));
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    glUseProgram(ShaderProgram);

    glBindVertexArray(VAO);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!MainWindow->ShouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        MainWindow->SwapBuffers();
    }
}

void Application::Shutdown()
{
    WindowingModule::Shutdown();
}

void Application::CreateMainWindow(WindowCreateInfo& WindowInfo)
{
    auto WindowCreateResult = Window::Create(WindowInfo);
    if (!WindowCreateResult)
    {
        LOG(Application, Fatal, "MainWindow creation failed");
    }

    MainWindow = WindowCreateResult.MoveValue();
}

void Application::CreateRenderer(RendererCreateInfo& RendererInfo)
{
    auto RendererCreateResult = Renderer::Create(RendererInfo);
    if (!RendererCreateResult)
    {
        //LOG(Application, Fatal, "Renderer creation failed")
    }

    Renderer = RendererCreateResult.MoveValue();
}
} // Oggle