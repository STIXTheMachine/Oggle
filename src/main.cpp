#include <iostream>
#include <print>
#include <Renderer/GraphicsLibs.hpp>
#include <Renderer/Vertex.hpp>
#include <Core/ColorPalettes.hpp>
#include <Renderer/ShaderProgramBuilder.hpp>
#include <glm/common.hpp>
#include <Core/TypeBases.hpp>

#define BUFFER_OFFSET(Offset) ((void*)(Offset))

const char* VertexShaderSource =
R"(
#version 450

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* FragmentShaderSource =
R"(
#version 450

out vec4 FragColor;
uniform vec4 InColor;

void main()
{
    FragColor = InColor;
}
)";

const char* ComputeShaderSource =
R"(
#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
asdf
    // do absolutely nothing
}
)";

enum {
    SUCCESS = 0,
    GLFW_INIT_FAILED,
    GLFW_MAIN_WINDOW_CREATION_FAILED,
};


/// Adapted from https://learnopengl.com/In-Practice/Debugging
void GLAPIENTRY LogOpenGLError(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const char* Message, const void* UserParam)
{
    if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204) return;

    std::println("================ [OpenGL Debug Message] ================");
    std::println("ID: {}", Id);

    switch (Source)
    {
        case GL_DEBUG_SOURCE_API:             std::println("Source: API");             break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::println("Source: Window System");   break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::println("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::println("Source: Third Party");     break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::println("Source: Application");     break;
        case GL_DEBUG_SOURCE_OTHER:           std::println("Source: Other");           break;
        default:                              std::println("Source: INVALID SOURCE");  break;
    }

    switch (Type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::println("Type: Error");               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::println("Type: Deprecated Behavior"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::println("Type: Undefined Behavior");  break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::println("Type: Portability");         break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::println("Type: Performance");         break;
        case GL_DEBUG_TYPE_MARKER:              std::println("Type: Marker");              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::println("Type: Push Group");          break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::println("Type: Pop Grou");            break;
        case GL_DEBUG_TYPE_OTHER:               std::println("Type: Other");               break;
        default:                                std::println("Type: INVALID TYPE");        break;
    }

    switch (Severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::println("Severity: High");             break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::println("Severity: Medium");           break;
        case GL_DEBUG_SEVERITY_LOW:          std::println("Severity: Low");              break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::println("Severity: Notification");     break;
        default:                             std::println("Severity: INVALID SEVERITY"); break;
    }

    std::println("{}", Message);
    std::println("========================================================");
}

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

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(LogOpenGLError, nullptr);

    const Vertex_P Triangle[]
    {
        {  0.0f,  0.5f, 0.0f }, // Top
        {  0.5f, -0.5f, 0.0f }, // Right
        { -0.5f, -0.5f, 0.0f }, // Left
    };

    const Vertex_P Cube[]
    {
        {  0.5f,  0.5f,  0.5f }, // Right top front
        {  0.5f,  0.5f, -0.5f }, // Right top back
        {  0.5f, -0.5f,  0.5f }, // Right bottom front
        {  0.5f, -0.5f, -0.5f }, // Right bottom back
        { -0.5f,  0.5f,  0.5f }, // Left top front
        { -0.5f,  0.5f, -0.5f }, // Left top back
        { -0.5f, -0.5f,  0.5f }, // Left bottom front
        { -0.5f, -0.5f, -0.5f }, // Left bottom back
    };

    ShaderProgramBuilder Compute;
    Compute.SetComputeShaderSource(ComputeShaderSource);
    GLuint ComputeProgram = Compute.Build();
    glUseProgram(ComputeProgram);
    glDispatchCompute(1, 1, 1);
    // if (GLenum E = glGetError(); E != GL_NO_ERROR)
    // {
    //     auto Eint = static_cast<unsigned int>(E);
    //     std::println("Error: {}", Eint);
    // }

    GLuint VAO, VBO, EBO;
    glCreateBuffers(1, &VBO);
    glCreateVertexArrays(1, &VAO);
    glNamedBufferStorage(VAO, sizeof(Triangle), Triangle, GL_NONE_BIT);

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex_P::Stride, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    unsigned int VertexShader   = glCreateShader(GL_VERTEX_SHADER);
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int ShaderProgram  = glCreateProgram();

    glShaderSource(VertexShader,   1, &VertexShaderSource, nullptr);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);

    glCompileShader(VertexShader);
    glCompileShader(FragmentShader);

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    auto TriangleColor = FloatColor { Palettes::Catppuccin::Mocha::Peach };
    auto InColorLoc = glGetUniformLocation(ShaderProgram, "InColor");
    glUseProgram(ShaderProgram);
    glUniform4f(InColorLoc, TriangleColor.R, TriangleColor.G, TriangleColor.B, TriangleColor.A);

    auto Background = FloatColor { Palettes::Catppuccin::Mocha::Base };
    glClearColor(Background.R, Background.G, Background.B, Background.A);

    double SwapInterval = 0.5;
    double SwapTime = glfwGetTime() + SwapInterval;

    while (!glfwWindowShouldClose(MainWindow)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (false && glfwGetTime() > SwapTime)
        {
            SwapTime += SwapInterval;

            GLint BoundVAO {};
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &BoundVAO);

            glBindVertexArray(BoundVAO == 0 ? VAO : 0);
        }

        glDrawArrays(GL_LINE_LOOP, 0, 3);
        
        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return SUCCESS;
}