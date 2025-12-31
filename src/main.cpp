#include <iostream>
#include <Renderer/GraphicsLibs.hpp>
#include <Renderer/Vertex.hpp>
#include <Core/ColorPalettes.hpp>
#include <Renderer/ShaderProgramBuilder.hpp>
#include <Renderer/LogOpenGL.hpp>
#include <Renderer/DataBuffer.hpp>
#include <format>
#include <print>

#define BUFFER_OFFSET(Offset) ((void*)(Offset))

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

    std::vector<std::byte> Foo {
        0xDE_b, 0xAD_b, 0xBE_b, 0xEF_b,
        0x01_b, 0x23_b, 0x45_b, 0x67_b,
        0x89_b, 0xAB_b, 0xCD_b, 0xEF_b,
        0x10_b, 0x20_b, 0x30_b, 0x40_b
    };

    DataBuffer Buf;
    Buf.AcquireData(Foo);

    LOG(Buf);
    LOG(FMT("Hello, {}", "World!"));

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

    std::filesystem::path VertexSourcePath   { "Triangle.vert" };
    std::filesystem::path FragmentSourcePath { "Triangle.frag" };

    const GLuint ShaderProgram =
        Builder
        .SetVertexSource(VertexSourcePath)
        .SetFragmentSource(FragmentSourcePath)
        .Build();

    auto TriangleColor = FloatColor { Palettes::Catppuccin::Mocha::Blue };
    auto InColorLoc = glGetUniformLocation(ShaderProgram, "InColor");

    glUseProgram(ShaderProgram);
    glUniform4f(InColorLoc, TriangleColor.R, TriangleColor.G, TriangleColor.B, TriangleColor.A);

    auto Background = FloatColor { Palettes::Catppuccin::Mocha::Base };
    glClearColor(Background.R, Background.G, Background.B, Background.A);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}