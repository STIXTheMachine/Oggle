//
// Created by jake on 12/28/25.
//
#include <Renderer/LogOpenGL.hpp>

DEFINE_LOG_CATEGORY(OpenGL)

/// Adapted from https://learnopengl.com/In-Practice/Debugging
void GLAPIENTRY LogOpenGLError(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const char* Message, const void* UserParam)
{
    using namespace Oggle::Logging;
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

    LOG(OpenGL, Verbosity, "================ [Begin OpenGL Debug Message] ================");
    LOGFMT(OpenGL, Verbosity, "ID: {}", Id);

    switch (Source)
    {
        case GL_DEBUG_SOURCE_API:             LOG(OpenGL, Verbosity, "Source: API");             break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOG(OpenGL, Verbosity, "Source: Window System");   break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: LOG(OpenGL, Verbosity, "Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     LOG(OpenGL, Verbosity, "Source: Third Party");     break;
        case GL_DEBUG_SOURCE_APPLICATION:     LOG(OpenGL, Verbosity, "Source: Application");     break;
        case GL_DEBUG_SOURCE_OTHER:           LOG(OpenGL, Verbosity, "Source: Other");           break;
        default:                              LOG(OpenGL, Verbosity, "Source: INVALID SOURCE");  break;
    }

    switch (Type)
    {
        case GL_DEBUG_TYPE_ERROR:               LOG(OpenGL, Verbosity, "Type: Error");               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOG(OpenGL, Verbosity, "Type: Deprecated Behavior"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOG(OpenGL, Verbosity, "Type: Undefined Behavior");  break;
        case GL_DEBUG_TYPE_PORTABILITY:         LOG(OpenGL, Verbosity, "Type: Portability");         break;
        case GL_DEBUG_TYPE_PERFORMANCE:         LOG(OpenGL, Verbosity, "Type: Performance");         break;
        case GL_DEBUG_TYPE_MARKER:              LOG(OpenGL, Verbosity, "Type: Marker");              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          LOG(OpenGL, Verbosity, "Type: Push Group");          break;
        case GL_DEBUG_TYPE_POP_GROUP:           LOG(OpenGL, Verbosity, "Type: Pop Grou");            break;
        case GL_DEBUG_TYPE_OTHER:               LOG(OpenGL, Verbosity, "Type: Other");               break;
        default:                                LOG(OpenGL, Verbosity, "Type: INVALID TYPE");        break;
    }

    switch (Severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         LOG(OpenGL, Verbosity, "Severity: High");             break;
        case GL_DEBUG_SEVERITY_MEDIUM:       LOG(OpenGL, Verbosity, "Severity: Medium");           break;
        case GL_DEBUG_SEVERITY_LOW:          LOG(OpenGL, Verbosity, "Severity: Low");              break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: LOG(OpenGL, Verbosity, "Severity: Notification");     break;
        default:                             LOG(OpenGL, Verbosity, "Severity: INVALID SEVERITY"); break;
    }

    LOGFMT(OpenGL, Verbosity, "{}", Message);
    LOG(OpenGL, Verbosity, "================= [End OpenGL Debug Message] =================");
}
