#include <iostream>
#include "Renderer/GraphicsLibs.hpp"
#include "Renderer/Vertex.hpp"
#include "Core/ColorPalettes.hpp"


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
        {  0.0f,  0.0f, 0.0f }, // Top
        {  0.5f, -0.5f, 0.0f }, // Right
        { -0.5f, -0.5f, 0.0f }, // Left
    };

    unsigned int VBO {};
    glCreateVertexArrays(1, &VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);


    auto Background = FloatColor { Palettes::Catppuccin::Mocha::Base };
    glClearColor(Background.R, Background.G, Background.B, Background.A);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}