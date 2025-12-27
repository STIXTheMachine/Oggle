#pragma once
#include <Renderer/GraphicsLibs.hpp>
#include "Core/Logging.hpp"

DECLARE_LOG_CATEGORY(OpenGL, Info, Default)

/// Adapted from https://learnopengl.com/In-Practice/Debugging
inline void GLAPIENTRY LogOpenGLError(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const char* Message, const void* UserParam)
{
    if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204) return;

    const ELogVerbosity Verbosity = [Severity]()
    {
        switch (Severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:   return ELogVerbosity::Error;
            case GL_DEBUG_SEVERITY_MEDIUM: return ELogVerbosity::Warning;
            case GL_DEBUG_SEVERITY_LOW:    return ELogVerbosity::Warning;
            default:                       return ELogVerbosity::Info;
        }
    }();

    Log<LogOpenGL>(Verbosity, "================ [Begin OpenGL Debug Message] ================");
    Log<LogOpenGL>(Verbosity, "ID: {}", Id);

    switch (Source)
    {
        case GL_DEBUG_SOURCE_API:             Log<LogOpenGL>(Verbosity, "Source: API");             break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Log<LogOpenGL>(Verbosity, "Source: Window System");   break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: Log<LogOpenGL>(Verbosity, "Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     Log<LogOpenGL>(Verbosity, "Source: Third Party");     break;
        case GL_DEBUG_SOURCE_APPLICATION:     Log<LogOpenGL>(Verbosity, "Source: Application");     break;
        case GL_DEBUG_SOURCE_OTHER:           Log<LogOpenGL>(Verbosity, "Source: Other");           break;
        default:                              Log<LogOpenGL>(Verbosity, "Source: INVALID SOURCE");  break;
    }

    switch (Type)
    {
        case GL_DEBUG_TYPE_ERROR:               Log<LogOpenGL>(Verbosity, "Type: Error");               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Log<LogOpenGL>(Verbosity, "Type: Deprecated Behavior"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Log<LogOpenGL>(Verbosity, "Type: Undefined Behavior");  break;
        case GL_DEBUG_TYPE_PORTABILITY:         Log<LogOpenGL>(Verbosity, "Type: Portability");         break;
        case GL_DEBUG_TYPE_PERFORMANCE:         Log<LogOpenGL>(Verbosity, "Type: Performance");         break;
        case GL_DEBUG_TYPE_MARKER:              Log<LogOpenGL>(Verbosity, "Type: Marker");              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          Log<LogOpenGL>(Verbosity, "Type: Push Group");          break;
        case GL_DEBUG_TYPE_POP_GROUP:           Log<LogOpenGL>(Verbosity, "Type: Pop Grou");            break;
        case GL_DEBUG_TYPE_OTHER:               Log<LogOpenGL>(Verbosity, "Type: Other");               break;
        default:                                Log<LogOpenGL>(Verbosity, "Type: INVALID TYPE");        break;
    }

    switch (Severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         Log<LogOpenGL>(Verbosity, "Severity: High");             break;
        case GL_DEBUG_SEVERITY_MEDIUM:       Log<LogOpenGL>(Verbosity, "Severity: Medium");           break;
        case GL_DEBUG_SEVERITY_LOW:          Log<LogOpenGL>(Verbosity, "Severity: Low");              break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: Log<LogOpenGL>(Verbosity, "Severity: Notification");     break;
        default:                             Log<LogOpenGL>(Verbosity, "Severity: INVALID SEVERITY"); break;
    }

    Log<LogOpenGL>(Verbosity, "{}", Message);
    Log<LogOpenGL>(Verbosity, "================= [End OpenGL Debug Message] =================");
}
