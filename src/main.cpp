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

#define offset(Offset) ((void*)(Offset))
#define countof(x) sizeof((x)) / sizeof((x[0]))

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

    struct Mesh
    {
        GLsizei NumVertexAttributes;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };

    // ======== Define/fetch data ========
    const Vertex_PC Vertices[]
    {
        Vertex_PC { Vec3 {  0.5f,  0.5,  0.5f }, FloatColor { Palettes::Catppuccin::Latte::Red } },
        Vertex_PC { Vec3 {  0.5f, -0.5,  0.5f }, FloatColor { Palettes::Catppuccin::Latte::Green } },
        Vertex_PC { Vec3 { -0.5f,  0.5,  0.5f }, FloatColor { Palettes::Catppuccin::Latte::Blue } },
        Vertex_PC { Vec3 { -0.5f, -0.5,  0.5f }, FloatColor { Palettes::Catppuccin::Latte::Surface0 } },

        Vertex_PC { Vec3 {  0.5f,  0.5, -0.5f }, FloatColor { Palettes::Catppuccin::Latte::Red } },
        Vertex_PC { Vec3 {  0.5f, -0.5, -0.5f }, FloatColor { Palettes::Catppuccin::Latte::Green } },
        Vertex_PC { Vec3 { -0.5f,  0.5, -0.5f }, FloatColor { Palettes::Catppuccin::Latte::Blue } },
        Vertex_PC { Vec3 { -0.5f, -0.5, -0.5f }, FloatColor { Palettes::Catppuccin::Latte::Surface0 } },
    };

    const GLuint Indices[] {
        0, 2, 1, 1, 2, 3,  // Back
        4, 5, 6, 5, 7, 6,  // Front
        0, 1, 5, 5, 4, 0,  // Right
        2, 7, 3, 6, 7, 2,  // Left
        0, 6, 2, 4, 6, 0,  // Top
        1, 3, 7, 7, 5, 1,  // Bottom
    };

    // ========= Create buffers/arrays ========
    GLuint VAO, VBO, EBO;

    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);
    glCreateVertexArrays(1, &VAO);

    // ======== Buffer mesh data ========
    glNamedBufferStorage(VBO, sizeof(Vertices), Vertices, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(EBO, sizeof(Indices), Indices, GL_DYNAMIC_STORAGE_BIT);

    // ======== Attach VBO and configure attribute ========
    glVertexArrayVertexBuffer(VAO, 0, VBO,                           0, sizeof(Vertex_PC)); // Position
    glVertexArrayVertexBuffer(VAO, 1, VBO, sizeof(Vertex_PC::Position), sizeof(Vertex_PC)); // Color

    // Position
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    // Color
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 4, GL_FLOAT, GL_TRUE, 0);
    glVertexArrayAttribBinding(VAO, 1, 1);

    glVertexArrayElementBuffer(VAO, EBO);

    // Shader
    ShaderProgramBuilder Builder;

    std::filesystem::path VertexSourcePath   { "Triangle.vert" };
    std::filesystem::path FragmentSourcePath { "Triangle.frag" };

    const GLuint ShaderProgram =
        Builder
        .SetVertexSource(VertexSourcePath)
        .SetFragmentSource(FragmentSourcePath)
        .Build();

    const auto BackgroundColor = FloatColor { Palettes::Catppuccin::Mocha::Base };

    const auto InTransformLoc = glGetUniformLocation(ShaderProgram, "inTransform");
    glm::mat4 Transform(1.0f);
    Transform = glm::translate(Transform, glm::vec3(0.0, 0.0, 0.0));

    glClearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);
    glPointSize(5.f);

    bool bRotating = true;

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(MainWindow)) {
        // if (glfwGetKey(MainWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        // {
        //     bRotating = false;
        // }
        // else
        // {
        //     bRotating = true;
        // }

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(ShaderProgram);

        if (bRotating)
        {
            Transform = glm::rotate(Transform, 0.0250f, glm::vec3(1.0f, 1.0f, 1.0f));
        }

        glUniformMatrix4fv(InTransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));

        glDrawElements(GL_TRIANGLES, std::size(Indices), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}