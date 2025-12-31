#include <iostream>
#include <Renderer/GraphicsLibs.hpp>
#include <Renderer/Vertex.hpp>
#include <Core/ColorPalettes.hpp>
#include <Renderer/ShaderProgramBuilder.hpp>
#include <Renderer/LogOpenGL.hpp>
#include <Renderer/DataBuffer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    #if BUILD_DEBUG
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    #endif

    auto MainWindow = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);
    if (!MainWindow) {
        return GLFW_MAIN_WINDOW_CREATION_FAILED;
    }

    glfwMakeContextCurrent(MainWindow);

    glbinding::initialize(glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(LogOpenGLError, nullptr);

    // Mesh
    const Vertex_P Vertices[]
    {
        {  0.0f,  0.577f, 0.0f }, // Top
        { -0.5f, -0.289f, 0.0f }, // Left
        {  0.5f, -0.289f, 0.0f }, // Right
        { 0.0f, 0.0f, 0.0f } // Origin
    };

    const GLuint Indices[] { 0, 1, 2, 3 };

    GLuint VAO, VBO, EBO;

    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, sizeof(Vertices), Vertices, GL_DYNAMIC_STORAGE_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex_P::Stride, BUFFER_OFFSET(0));

    glCreateBuffers(1, &EBO);
    glNamedBufferStorage(EBO, sizeof(Indices), Indices, GL_DYNAMIC_STORAGE_BIT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Shader
    ShaderProgramBuilder Builder;

    std::filesystem::path VertexSourcePath   { "Triangle.vert" };
    std::filesystem::path FragmentSourcePath { "Triangle.frag" };

    const GLuint ShaderProgram =
        Builder
        .SetVertexSource(VertexSourcePath)
        .SetFragmentSource(FragmentSourcePath)
        .Build();

    const auto TriangleColor   = FloatColor { Palettes::Catppuccin::Mocha::Sapphire };
    const auto PointColor      = FloatColor { Palettes::Catppuccin::Mocha::Crust };
    const auto BackgroundColor = FloatColor { Palettes::Catppuccin::Mocha::Surface2 };

    const auto InColorLoc = glGetUniformLocation(ShaderProgram, "InColor");
    const auto InTransformLoc = glGetUniformLocation(ShaderProgram, "inTransform");
    glm::mat4 Transform(1.0f);

    glClearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);
    glUseProgram(ShaderProgram);
    glPointSize(5.f);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        Transform = glm::rotate(Transform, 0.0250f, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(InTransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));

        glUniform4f(InColorLoc, TriangleColor.R, TriangleColor.G, TriangleColor.B, TriangleColor.A);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glUniform4f(InColorLoc, PointColor.R, PointColor.G, PointColor.B, PointColor.A);
        glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}