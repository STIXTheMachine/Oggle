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

    struct Mesh
    {
        Mesh() = default;

        void CreateBuffers() {
            glCreateBuffers(1, &VertexBuffer);

            VertexArrays.reserve(NumVertexAttributes);
            glCreateVertexArrays(NumVertexAttributes, VertexArrays.data());

            ElementArrays.reserve(NumVertexAttributes);
            glCreateBuffers(NumVertexAttributes, ElementArrays.data());
        }

        void Init (GLsizei InNumVertexAttributes, BufferView Data)
        {
            NumVertexAttributes = InNumVertexAttributes;
            CreateBuffers();
        }

        void Draw()
        {
            //glBindVertexArray(VAO);

        }

        GLsizei NumVertexAttributes;
        GLuint VertexBuffer;
        GLuint ElementBuffer;
        TDynamicArray<GLuint> VertexArrays;
        TDynamicArray<GLuint> ElementArrays;
        DataBuffer MeshData;
    };

    // ======== Define/fetch data ========
    const Vertex_PC Vertices[]
    {
        Vertex_PC { Vec3 {  0.0f,  0.577f, 0.0f }, FloatColor { Palettes::Catppuccin::Mocha::Mauve } }, // Top
        Vertex_PC { Vec3 { -0.5f, -0.289f, 0.0f }, FloatColor { Palettes::Catppuccin::Mocha::Peach } }, // Left
        Vertex_PC { Vec3 {  0.5f, -0.289f, 0.0f }, FloatColor { Palettes::Catppuccin::Mocha::Blue } }, // Right
        Vertex_PC { Vec3 {  0.0f,  0.0f,   0.0f }, FloatColor { Palettes::Catppuccin::Mocha::Red } } // Origin
    };

    const GLuint Indices[] { 0, 1, 2, 3 };

    // ========= Create buffers/arrays ========
    GLuint VAO, VBO, EBO;

    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);
    glCreateVertexArrays(1, &VAO);

    // ======== Buffer mesh data ========
    glNamedBufferStorage(VBO, sizeof(Vertices), Vertices, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(EBO, sizeof(Indices), Indices, GL_DYNAMIC_STORAGE_BIT);

    // ======== Attach VBO and configure attribute ========
    glVertexArrayVertexBuffer(VAO, 0, VBO,                      0, sizeof(Vertex_PC));
    glVertexArrayVertexBuffer(VAO, 1, VBO, sizeof(Vertex_PC::Pos), sizeof(Vertex_PC));

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

    glClearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);
    glPointSize(5.f);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(ShaderProgram);

        Transform = glm::rotate(Transform, 0.0250f, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(InTransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}