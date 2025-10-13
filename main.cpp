#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

#include "glbinding/glbinding.h"
using namespace gl;

struct GLFWLib {
    static void Init() {
        static GLFWLib instance;
    }

    static bool isInitialized() {
        return glfwInit() == GLFW_TRUE;
    }

    ~GLFWLib() {
        glfwTerminate();
    }

private:
    GLFWLib() {
        glfwInit();
    }
};

// Anonymous enum inside namespace to preserve scoping without having to static_cast<int>(ReturnValues::Foo) everywhere.
namespace ReturnValues {
    enum {
        SUCCESS = 0,
        GLFW_INIT_FAILED,
        GLFW_MAIN_WINDOW_CREATION_FAILED,
    };
}

int main() {

    GLFWLib::Init();

    if (!GLFWLib::isInitialized()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return ReturnValues::GLFW_INIT_FAILED;
    }

    auto MainWindow = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!MainWindow) {
        return ReturnValues::GLFW_MAIN_WINDOW_CREATION_FAILED;
    }

    glfwMakeContextCurrent(MainWindow);

    glbinding::initialize(glfwGetProcAddress);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(MainWindow);

        glfwPollEvents();
    }

    return ReturnValues::SUCCESS;
}