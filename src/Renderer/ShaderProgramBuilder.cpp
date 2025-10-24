#include <fstream>
#include <print>
#include <Renderer/ShaderProgramBuilder.hpp>

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
    std::ifstream                  FileStream { NewSourceFile };
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

bool ShaderProgramBuilder::IsValidConfiguration() const
{
    const bool HasComputeShader  = ComputeShaderSource.HasSource();
    const bool HasAnyOtherShader =
            VertexShaderSource.HasSource()
            || TessControlShaderSource.HasSource()
            || TessEvalShaderSource.HasSource()
            || GeometryShaderSource.HasSource()
            || FragmentShaderSource.HasSource();
    const bool IsValidComputeShader = HasComputeShader && !HasAnyOtherShader;

    const bool HasVertexAndFragShader   = VertexShaderSource.HasSource() && FragmentShaderSource.HasSource();
    const bool HasZeroOrBothTessShaders = (TessControlShaderSource.HasSource() == TessEvalShaderSource.HasSource());
    const bool IsValidGraphicsShader    = HasVertexAndFragShader && HasZeroOrBothTessShaders && !HasComputeShader;

    return IsValidGraphicsShader || IsValidComputeShader;
}

GLuint ShaderProgramBuilder::Build()
{
    if (!IsValidConfiguration())
    {
        // TODO: make this more descriptive
        std::println("WARNING: Attempting to build invalid shader program.");
        return 0;
    }

    GLuint Vertex {}, TessControl {}, TessEval {}, Geometry {}, Fragment {}, Compute {};
    GLuint Program = glCreateProgram();

    if (VertexShaderSource.HasSource())
    {
        Vertex = CompileShader(VertexShaderSource, GL_VERTEX_SHADER);
        glAttachShader(Program, Vertex);
    }
    if (TessControlShaderSource.HasSource())
    {
        TessControl = CompileShader(TessControlShaderSource, GL_TESS_CONTROL_SHADER);
        glAttachShader(Program, TessControl);
    }
    if (TessEvalShaderSource.HasSource())
    {
        TessEval = CompileShader(TessEvalShaderSource, GL_TESS_EVALUATION_SHADER);
        glAttachShader(Program, TessEval);
    }
    if (GeometryShaderSource.HasSource())
    {
        Geometry = CompileShader(GeometryShaderSource, GL_GEOMETRY_SHADER);
        glAttachShader(Program, Geometry);
    }
    if (FragmentShaderSource.HasSource())
    {
        Fragment = CompileShader(FragmentShaderSource, GL_FRAGMENT_SHADER);
        glAttachShader(Program, Fragment);
    }
    if (ComputeShaderSource.HasSource())
    {
        Compute = CompileShader(ComputeShaderSource, GL_GEOMETRY_SHADER);
        glAttachShader(Program, Compute);
    }

    glLinkProgram(Program);

    for (const auto& Shader : { Vertex, TessControl, TessEval, Geometry, Fragment, Compute })
    {
        if (glIsShader(Shader))
        {
            glDeleteShader(Shader);
        }
    }

    return Program;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetVertexSource(const Path& NewSourcePath)
{
    VertexShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetVertexSource(std::string_view NewSourceString)
{
    VertexShaderSource.SetFromString(NewSourceString);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetVertexSource(const char* NewSourceString)
{
    VertexShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessControlSource(const Path& NewSourcePath)
{
    TessControlShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessControlSource(std::string_view NewSourceString)
{
    TessControlShaderSource.SetFromString(NewSourceString);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessControlSource(const char* NewSourceString)
{
    TessControlShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessEvalSource(const Path& NewSourcePath)
{
    TessEvalShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessEvalSource(std::string_view NewSourceString)
{
    TessControlShaderSource.SetFromString(NewSourceString);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetTessEvalSource(const char* NewSourceString)
{
    TessEvalShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetGeometrySource(const Path& NewSourcePath)
{
    GeometryShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetGeometrySource(std::string_view NewSourceString)
{
    GeometryShaderSource.SetFromString(NewSourceString);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetGeometrySource(const char* NewSourceString)
{
    GeometryShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetFragmentSource(const Path& NewSourcePath)
{
    FragmentShaderSource.SetFromFile(NewSourcePath);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetFragmentSource(std::string_view NewSourceString)
{
    FragmentShaderSource.SetFromString(NewSourceString);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetFragmentSource(const char* NewSourceString)
{
    FragmentShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::SetComputeShaderSource(const Path& NewSourcePath)
{
    ComputeShaderSource.SetFromFile(NewSourcePath);
    return *this;
}
ShaderProgramBuilder& ShaderProgramBuilder::SetComputeShaderSource(std::string_view NewSourceString)
{
    ComputeShaderSource.SetFromString(NewSourceString);
    return *this;
}
ShaderProgramBuilder& ShaderProgramBuilder::SetComputeShaderSource(const char* NewSourceString)
{
    ComputeShaderSource.SetFromString( std::string_view { NewSourceString } );
    return *this;
}

GLuint ShaderProgramBuilder::CompileShader(const ShaderSource& Source, GLenum Type)
{
    const GLuint Shader = glCreateShader(Type);
    const char*  Temp   = Source.GetSourceString().data();

    glShaderSource(Shader, 1, &Temp, nullptr);
    glCompileShader(Shader);

    GLboolean Success;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

    if (!Success)
    {
        PrintInfoLog(Shader);

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

void ShaderProgramBuilder::PrintInfoLog(GLuint ShaderOrProgram)
{
    if (glIsShader(ShaderOrProgram))
    {
        char InfoLog[1024];
        glGetShaderInfoLog(ShaderOrProgram, 1024, nullptr, InfoLog);

        std::println(
                     "=========================================================\n"
                     "ERROR: failed to compile shader!\n"
                     "================ [OpenGL Shader InfoLog] ================\n"
                     "{}"
                     "=========================================================\n",
                     InfoLog
                    );
    }
    else if (glIsProgram(ShaderOrProgram))
    {
        char InfoLog[1024];
        glGetProgramInfoLog(ShaderOrProgram, 1024, nullptr, InfoLog);

        std::println(
                     "==========================================================\n"
                     "ERROR: failed to link program!\n"
                     "================ [OpenGL Program InfoLog] ================\n"
                     "{}"
                     "==========================================================\n",
                     InfoLog
                    );
    }
}
