#include <iostream>
#include <print>
#include "Renderer/GraphicsLibs.hpp"
#include "Renderer/Vertex.hpp"
#include "Core/ColorPalettes.hpp"

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

enum {
    SUCCESS = 0,
    GLFW_INIT_FAILED,
    GLFW_MAIN_WINDOW_CREATION_FAILED,
};

int main() {

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

    const Vertex_P Triangle []
    {
        {  0.0f,  0.5f, 0.0f }, // Top
        {  0.5f, -0.5f, 0.0f }, // Right
        { -0.5f, -0.5f, 0.0f }, // Left
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

    unsigned int VertexShader   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int ShaderProgram  = glCreateProgram();

    glShaderSource(VertexShader,   1, &VertexShaderSource, nullptr);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

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