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
#include <Renderer/BitmapLoader.hpp>
#include "Renderer/Camera.hpp"

enum {
    SUCCESS = 0,
    GLFW_INIT_FAILED,
    GLFW_MAIN_WINDOW_CREATION_FAILED,
};

static Oggle::Camera Camera { glm::vec3(0.0f, 0.0f, -5.f) };

static struct
{
    int Width = 640;
    int Height = 480;
    std::string_view Name = "Oggle";
} WindowSettings;

void HandleMouseInput(GLFWwindow* Window, double X, double Y)
{
    static double LastX {};
    static double LastY {};
    static bool   bNeedsInit = true;

    if (bNeedsInit)
    {
        LastX = X;
        LastY = Y;
        bNeedsInit = false;
        return;
    }

    const double DeltaX = X - LastX;
    const double DeltaY = Y - LastY;
    LastX = X;
    LastY = Y;

    const float Sensitivity = 0.1f;

    Camera.PitchUp(DeltaY * Sensitivity);
    Camera.YawRight(-DeltaX * Sensitivity);
}

void HandleKeyInput(GLFWwindow* Window)
{
    static float LastFrameTime;
    static bool  bNeedsInit = true;

    if (bNeedsInit)
    {
        LastFrameTime = glfwGetTime();
        bNeedsInit = false;
        return;
    }

    const float CurrentTime = glfwGetTime();
    const float DeltaTime = CurrentTime - LastFrameTime;
    LastFrameTime = CurrentTime;

    constexpr float CameraSpeed = 5.f;
    const float MovementDelta = CameraSpeed * DeltaTime;

    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Camera.TranslateLocal(glm::vec3(0.f, 0.f, MovementDelta));
    }
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Camera.TranslateLocal(glm::vec3(0.f, 0.f, -MovementDelta));
    }
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Camera.TranslateLocal(glm::vec3(MovementDelta, 0.f, 0.f));
    }
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Camera.TranslateLocal(glm::vec3(-MovementDelta, 0.f, 0.f));
    }
    if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Camera.Translate(glm::vec3(0.f, MovementDelta, 0.f));
    }
    if (glfwGetKey(Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Camera.Translate(glm::vec3(0.f, -MovementDelta, 0.f));
    }
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, true);
    }
}

int main()
{
    Oggle::BitmapLoader Loader;
    Loader.Load("Assets/Textures/Grumbus.bmp");

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

    glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(MainWindow, HandleMouseInput);
    //glfwSetKeyCallback(MainWindow, HandleKeyInput);

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
    glm::mat4 Projection = glm::perspectiveFov(glm::radians(Camera.VerticalFOV), (float)WindowSettings.Width, (float)WindowSettings.Height, 0.1f, 100.0f);
    glm::mat4 Model(1.0f);
    Model = glm::translate(Model, glm::vec3(0.0, 0.0, -1));

    glClearColor(BackgroundColor.R, BackgroundColor.G, BackgroundColor.B, BackgroundColor.A);
    glPointSize(5.f);


    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(MainWindow)) {
        HandleKeyInput(MainWindow);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ShaderProgram);

        //Model = glm::rotate(Model, 0.0250f, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(InTransformLoc, 1, GL_FALSE, glm::value_ptr(Projection * Camera.GetViewMatrix() * Model));

        Cube.Draw();

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}