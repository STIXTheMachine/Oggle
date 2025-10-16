#include <iostream>
#include "Renderer/GraphicsLibs.hpp"
#include "Renderer/VertexTypes.hpp"

enum {
    RV_SUCCESS = 0,
    RV_GLFW_INIT_FAILED,
    RV_GLFW_MAIN_WINDOW_CREATION_FAILED,
};

int main() {
    Vertex_PN Test {
        {
            VertexPosition { 0.0f, 0.0f, 0.0f },
            VertexNormal   { 1.0f, 0.0f, 0.0f }
        }
    };

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return RV_GLFW_INIT_FAILED;
    }

    auto MainWindow = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!MainWindow) {
        return RV_GLFW_MAIN_WINDOW_CREATION_FAILED;
    }

    glfwMakeContextCurrent(MainWindow);

    glbinding::initialize(glfwGetProcAddress);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(MainWindow);

        glfwPollEvents();
    }

    glfwTerminate();

    return RV_SUCCESS;
}