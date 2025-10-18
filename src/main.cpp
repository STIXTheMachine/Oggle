#include <iostream>
#include "Renderer/GraphicsLibs.hpp"
#include "Renderer/Vertex.hpp"
#include "Renderer/Color.hpp"

namespace Palettes
{
namespace Catppuccin
{
    enum Mocha
    {
        Rosewater = 0xF5E0DC,
        Flamingo  = 0xF2CDCD,
        Pink      = 0xF5C2E7,
        Mauve     = 0xCBA6F7,
        Red       = 0xF38BA8,
        Maroon    = 0xEBA0AC,
        Peach     = 0xFAB387,
        Yellow    = 0xF9E2AF,
        Green     = 0xA6E3A1,
        Teal      = 0x94E2D5,
        Sky       = 0x89DCEB,
        Sapphire  = 0x74C7EC,
        Blue      = 0x89B4FA,
        Lavender  = 0xB4BEFE,
        Text      = 0xCDD6F4,
        Subtext1  = 0xBAC2DE,
        Subtext0  = 0xA6ADC8,
        Overlay2  = 0x9399B2,
        Overlay1  = 0x7F849C,
        Overlay0  = 0x6C7086,
        Surface2  = 0x585B70,
        Surface1  = 0x45475A,
        Surface0  = 0x313244,
        Base      = 0x1E1E2E,
        Mantle    = 0x181825,
        Crust     = 0x11111B
    };
}
}

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

    while (!glfwWindowShouldClose(MainWindow)) {
        glClearColor(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}