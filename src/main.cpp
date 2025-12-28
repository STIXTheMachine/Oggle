#include <iostream>
#include <Renderer/GraphicsLibs.hpp>
#include <Renderer/Vertex.hpp>
#include <Core/ColorPalettes.hpp>
#include <Renderer/ShaderProgramBuilder.hpp>
#include <glm/common.hpp>
#include <Renderer/LogOpenGL.hpp>

#define BUFFER_OFFSET(Offset) ((void*)(Offset))

const char* VertexShaderSource =
R"(
#version 450

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* FragmentShaderSource =
R"(
#version 450

out vec4 FragColor;
uniform vec4 InColor;

void main()
{
    FragColor = InColor;
}
)";

const char* ComputeShaderSource =
R"(
#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
    // do absolutely nothing
}
)";

enum {
    SUCCESS = 0,
    GLFW_INIT_FAILED,
    GLFW_MAIN_WINDOW_CREATION_FAILED,
};

int main()
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return GLFW_INIT_FAILED;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    #if BUILD_DEBUG
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    #endif

    auto MainWindow = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!MainWindow) {
        return GLFW_MAIN_WINDOW_CREATION_FAILED;
    }

    glfwMakeContextCurrent(MainWindow);

    glbinding::initialize(glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(LogOpenGLError, nullptr);

    const Vertex_P Triangle[]
    {
        {  0.0f,  0.5f, 0.0f }, // Top
        {  0.5f, -0.5f, 0.0f }, // Right
        { -0.5f, -0.5f, 0.0f }, // Left
    };

    const Vertex_P Cube[]
    {
        {  0.5f,  0.5f,  0.5f }, // Right top front
        {  0.5f,  0.5f, -0.5f }, // Right top back
        {  0.5f, -0.5f,  0.5f }, // Right bottom front
        {  0.5f, -0.5f, -0.5f }, // Right bottom back
        { -0.5f,  0.5f,  0.5f }, // Left top front
        { -0.5f,  0.5f, -0.5f }, // Left top back
        { -0.5f, -0.5f,  0.5f }, // Left bottom front
        { -0.5f, -0.5f, -0.5f }, // Left bottom back
    };


    GLuint VAO, VBO, EBO;
    glCreateBuffers(1, &VBO);
    glCreateVertexArrays(1, &VAO);
    glNamedBufferStorage(VAO, sizeof(Triangle), Triangle, GL_NONE_BIT);

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex_P::Stride, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    ShaderProgramBuilder Builder;

    const GLuint ShaderProgram =
        Builder
        .VertexSource(VertexShaderSource)
        .FragmentSource(FragmentShaderSource)
        .Build();

    auto TriangleColor = FloatColor { Palettes::Catppuccin::Mocha::Peach };
    auto InColorLoc = glGetUniformLocation(ShaderProgram, "InColor");
    glUseProgram(ShaderProgram);
    glUniform4f(InColorLoc, TriangleColor.R, TriangleColor.G, TriangleColor.B, TriangleColor.A);

    auto Background = FloatColor { Palettes::Catppuccin::Mocha::Base };
    glClearColor(Background.R, Background.G, Background.B, Background.A);

    double SwapInterval = 0.5;
    double SwapTime = glfwGetTime() + SwapInterval;

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetTime() > SwapTime)
        {
            SwapTime += SwapInterval;

            GLint BoundVAO {};
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &BoundVAO);

            glBindVertexArray(BoundVAO == 0 ? VAO : 0);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}