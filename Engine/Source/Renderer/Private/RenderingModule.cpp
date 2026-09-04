#include "RenderingModule.hpp"
#include "LogRenderer.hpp"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

void Oggle::RenderingModule::Init()
{
    const int Version = gladLoadGL(glfwGetProcAddress);
    LOG(Renderer, FMT("Initializing OpenGL {}.{}...", GLAD_VERSION_MAJOR(Version), GLAD_VERSION_MINOR(Version)));
}

void Oggle::RenderingModule::Shutdown()
{

}
