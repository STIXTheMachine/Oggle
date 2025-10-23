#include <fstream>
#include <print>
#include <Renderer/ShaderProgramBuilder.hpp>

namespace Detail
{
GLuint CompileShader(const ShaderSource& Source, GLenum Type)
    {
        const GLuint Shader = glCreateShader(Type);
        const char*  Temp   = Source.GetSourceString().data();

        glShaderSource(Shader, 1, &Temp, nullptr);
        glCompileShader(Shader);

        GLboolean Success;
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

        if (!Success)
        {
            char InfoLog[1024];
            glGetShaderInfoLog(Shader, 1024, nullptr, InfoLog);

            // TODO: Check ShaderSource filepath and print it if present.
            std::println(
                "ERROR: failed to compile compute shader! Info log:\n{}",
                InfoLog
            );

            if (Source.GetSourceFilePath().has_value())
            {
                auto FullPathString = std::filesystem::canonical(Source.GetSourceFilePath().value()).string();
                std::println(
                    "In file: {}",
                    FullPathString
                );
            }

            glDeleteShader(Shader);
            return 0;
        }

        return Shader;
    }
}

void ShaderSource::SetFromFile(const std::filesystem::path& NewSourceFile)
{
    if (!std::filesystem::exists(NewSourceFile))
    {
        std::println(
            "ERROR: failed to load shader source file {}. File does not exist!",
            std::filesystem::canonical(NewSourceFile).string()
        );

        bHasSource = false;
    }

    SourceFile = NewSourceFile;
    std::ifstream FileStream { NewSourceFile };
    const std::istreambuf_iterator It { FileStream };

    std::string Temp { It, {} };
    std::swap(SourceString, Temp);

    bHasSource = true;
}

void ShaderSource::SetFromString(const std::string_view NewSourceString)
{
    std::string Temp { NewSourceString };
    std::swap(SourceString, Temp);
    SourceFile.reset();
    bHasSource = true;
}

void ShaderSource::Reset()
{
    SourceString.clear();
    SourceFile.reset();
    bHasSource = false;
}

bool GraphicsShaderProgramBuilder::IsValidConfiguration()
{
    const bool HasVertexAndFragShader = VertexShaderSource.HasSource() && FragmentShaderSource.HasSource();
    const bool HasZeroOrBothTessShaders = !(TessControlShaderSource.HasSource() ^ TessEvalShaderSource.HasSource());
    return HasVertexAndFragShader && HasZeroOrBothTessShaders;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetVertexSource(const Path& NewSourcePath)
{
    VertexShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetVertexSource(std::string_view NewSourceString)
{
    VertexShaderSource.SetFromString(NewSourceString);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetTessControlSource(const Path& NewSourcePath)
{
    TessControlShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetTessControlSource(std::string_view NewSourceString)
{
    TessControlShaderSource.SetFromString(NewSourceString);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetTessEvalSource(const Path& NewSourcePath)
{
    TessEvalShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetTessEvalSource(std::string_view NewSourceString)
{
    TessControlShaderSource.SetFromString(NewSourceString);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetGeometrySource(const Path& NewSourcePath)
{
    GeometryShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetGeometrySource(std::string_view NewSourceString)
{
    GeometryShaderSource.SetFromString(NewSourceString);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetFragmentSource(const Path& NewSourcePath)
{
    FragmentShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

GraphicsShaderProgramBuilder& GraphicsShaderProgramBuilder::SetFragmentSource(std::string_view NewSourceString)
{
    FragmentShaderSource.SetFromString(NewSourceString);
    return *this;
}

GLuint ComputeShaderProgramBuilder::Build()
{
    GLuint Program = glCreateProgram();

    GLuint Shader = Detail::CompileShader(Source, GL_COMPUTE_SHADER);

    if (Shader == 0)
    {
        glDeleteShader(Shader);
        glDeleteProgram(Program);
        return 0;
    }

    glAttachShader(Program, Shader);

    glDeleteShader(Shader);
    return Program;
}

bool ComputeShaderProgramBuilder::IsValidConfiguration()
{
    return Source.HasSource();
}

ComputeShaderProgramBuilder& ComputeShaderProgramBuilder::SetSource(const Path& InSource)
{
    Source.SetFromFile(InSource);
    return *this;
}

ComputeShaderProgramBuilder& ComputeShaderProgramBuilder::SetSource(std::string_view InString)
{
    Source.SetFromString(InString);
    return *this;
}
