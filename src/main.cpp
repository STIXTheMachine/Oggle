#include <iostream>
#include <Renderer/GraphicsLibs.hpp>
#include <Renderer/Vertex.hpp>
#include <Core/ColorPalettes.hpp>
#include <Renderer/ShaderProgramBuilder.hpp>
#include <Renderer/LogOpenGL.hpp>
#include <Renderer/DataBuffer.hpp>
#include <Renderer/Mesh.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum {
    SUCCESS = 0,
    GLFW_INIT_FAILED,
    GLFW_MAIN_WINDOW_CREATION_FAILED,
};

static struct
{
    glm::vec3 Position { 0.f, 3.f, -6.f };
    glm::quat Orientation {};
    float FOV { 30.f };
} Camera;

static struct
{
    int Width = 640;
    int Height = 480;
    std::string_view Name = "Oggle";
} WindowSettings;

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

    auto MainWindow = glfwCreateWindow(WindowSettings.Width, WindowSettings.Height, WindowSettings.Name.data(), nullptr, nullptr);
    if (!MainWindow) {
        return GLFW_MAIN_WINDOW_CREATION_FAILED;
    }

    glfwMakeContextCurrent(MainWindow);

    glbinding::initialize(glfwGetProcAddress);

#if BUILD_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(LogOpenGLError, nullptr);
#endif

    const Vertex_PC Vertices[]
    {
        Vertex_PC { {  0.5f,  0.5,  0.5f }, Palettes::Catppuccin::Latte::Red.ToFloatColor()      },
        Vertex_PC { {  0.5f, -0.5,  0.5f }, Palettes::Catppuccin::Latte::Green.ToFloatColor()    },
        Vertex_PC { { -0.5f,  0.5,  0.5f }, Palettes::Catppuccin::Latte::Blue.ToFloatColor()     },
        Vertex_PC { { -0.5f, -0.5,  0.5f }, Palettes::Catppuccin::Latte::Surface0.ToFloatColor() },

        Vertex_PC { {  0.5f,  0.5, -0.5f }, Palettes::Catppuccin::Latte::Red.ToFloatColor()      },
        Vertex_PC { {  0.5f, -0.5, -0.5f }, Palettes::Catppuccin::Latte::Green.ToFloatColor()    },
        Vertex_PC { { -0.5f,  0.5, -0.5f }, Palettes::Catppuccin::Latte::Blue.ToFloatColor()     },
        Vertex_PC { { -0.5f, -0.5, -0.5f }, Palettes::Catppuccin::Latte::Surface0.ToFloatColor() },
    };

    const GLuint VertexIndices[] {
        0, 2, 1, 1, 2, 3,  // Back
        4, 5, 6, 5, 7, 6,  // Front
        0, 1, 5, 5, 4, 0,  // Right
        2, 7, 3, 6, 7, 2,  // Left
        0, 6, 2, 4, 6, 0,  // Top
        1, 3, 7, 7, 5, 1,  // Bottom
    };

    Mesh Cube {};

    Cube.LoadData(Vertices, VertexIndices);

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

    const auto InTransformLoc = glGetUniformLocation(ShaderProgram, "InMVP");
    glm::mat4 View = glm::lookAt(Camera.Position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 Projection = glm::perspectiveFov(glm::radians(Camera.FOV), (float)WindowSettings.Width, (float)WindowSettings.Height, 0.1f, 100.0f);
    glm::mat4 Model(1.0f);
    Model = glm::translate(Model, glm::vec3(0.0, 0.0, -1));

    glClearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);
    glPointSize(5.f);


    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderProgram);

        Model = glm::rotate(Model, 0.0250f, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(InTransformLoc, 1, GL_FALSE, glm::value_ptr(Projection * View * Model));

        Cube.Draw();

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}