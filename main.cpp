#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>

#include "glbinding/glbinding.h"
using namespace gl;

enum {
    RV_SUCCESS = 0,
    RV_GLFW_INIT_FAILED,
    RV_GLFW_MAIN_WINDOW_CREATION_FAILED,
};

template<typename ScalarType, unsigned int Dimension>
using TVector = std::array<ScalarType, Dimension>;

using Vec2 = TVector<GLfloat, 2>;
using Vec3 = TVector<GLfloat, 3>;
using Vec4 = TVector<GLfloat, 4>;

template<typename AttribType, unsigned int N>
using TAttribArray = std::array<AttribType, N>;

int main() {


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