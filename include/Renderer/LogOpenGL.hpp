#pragma once
#include <Renderer/GraphicsLibs.hpp>
#include "Core/Logging/Logging.hpp"

DECLARE_LOG_CATEGORY(OpenGL, Info, Default)

void GLAPIENTRY LogOpenGLError(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const char* Message, const void* UserParam);