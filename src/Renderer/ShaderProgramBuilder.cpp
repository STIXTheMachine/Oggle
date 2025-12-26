#include <fstream>
#include <print>
#include <Renderer/ShaderProgramBuilder.hpp>

#include "Core/Tuple.hpp"

void ShaderSource::Load(const std::filesystem::path& NewSourceFile, GLenum InType)
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
    Type = InType;
}

void ShaderSource::Load(const std::string_view NewSourceString, GLenum InType)
{
    std::string Temp { NewSourceString };
    std::swap(SourceString, Temp);
    SourceFile.reset();
    bHasSource = true;
    Type = InType;
}

void ShaderSource::Reset()
{
    SourceString.clear();
    SourceFile.reset();
    bHasSource = false;
    Type = GL_INVALID_ENUM;
}

bool ShaderProgramBuilder::IsValidConfiguration() const
{
    const bool HasComputeShader      = Compute.HasSource();
    const bool HasNonComputeShader   = Vertex.HasSource()
                                    || TessControl.HasSource()
                                    || TessEval.HasSource()
                                    || Geometry.HasSource()
                                    || Fragment.HasSource();
    const bool IsValidComputeShader  = HasComputeShader && !HasNonComputeShader;

    const bool HasVertexAndFragShader   = Vertex.HasSource() && Fragment.HasSource();
    const bool HasZeroOrBothTessShaders = (TessControl.HasSource() == TessEval.HasSource());
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

    const GLuint Program = glCreateProgram();

    TTuple<const ShaderSource&, GLuint> Shaders[] =  {
        { Vertex, 0 },
        { TessControl, 0 },
        { TessEval, 0 },
        { Geometry, 0 },
        { Fragment, 0 },
        { Compute, 0 }
    };

    for (auto& [Source, ID] : Shaders)
    {
        if (Source.HasSource())
        {
            ID = CompileShader(Source);
            glAttachShader(Program, ID);
        }
    }

    glLinkProgram(Program);

    for (auto& [Source, ID] : Shaders)
    {
        if (glIsShader(ID))
        {
            glDeleteShader(ID);
        }
    }

    return Program;
}

void ShaderProgramBuilder::Reset()
{
    Vertex.Reset();
    TessControl.Reset();
    TessEval.Reset();
    Geometry.Reset();
    Fragment.Reset();
    Compute.Reset();
}

ShaderProgramBuilder& ShaderProgramBuilder::VertexSource(const Path& NewSourcePath)
{
    Vertex.Load(NewSourcePath, GL_VERTEX_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::VertexSource(std::string_view NewSourceString)
{
    Vertex.Load(NewSourceString, GL_VERTEX_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::VertexSource(const char* NewSourceString)
{
    Vertex.Load(std::string_view { NewSourceString } , GL_VERTEX_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::VertexSource(const ShaderSource& InSource)
{
    Vertex = InSource;
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessControlSource(const Path& NewSourcePath)
{
    TessControl.Load(NewSourcePath, GL_TESS_CONTROL_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessControlSource(std::string_view NewSourceString)
{
    TessControl.Load(NewSourceString, GL_TESS_CONTROL_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessControlSource(const char* NewSourceString)
{
    TessControl.Load(std::string_view { NewSourceString } , GL_TESS_CONTROL_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessControlSource(const ShaderSource& InSource)
{
    TessControl = InSource;
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessEvalSource(const Path& NewSourcePath)
{
    TessEval.Load(NewSourcePath, GL_TESS_EVALUATION_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessEvalSource(std::string_view NewSourceString)
{
    TessControl.Load(NewSourceString, GL_TESS_EVALUATION_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessEvalSource(const char* NewSourceString)
{
    TessEval.Load(std::string_view { NewSourceString } , GL_TESS_EVALUATION_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::TessEvalSource(const ShaderSource& InSource)
{
    TessEval = InSource;
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::GeometrySource(const Path& NewSourcePath)
{
    Geometry.Load(NewSourcePath, GL_GEOMETRY_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::GeometrySource(std::string_view NewSourceString)
{
    Geometry.Load(NewSourceString, GL_GEOMETRY_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::GeometrySource(const char* NewSourceString)
{
    Geometry.Load(std::string_view { NewSourceString } , GL_GEOMETRY_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::GeometrySource(const ShaderSource& InSource)
{
    Geometry = InSource;
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::FragmentSource(const Path& NewSourcePath)
{
    Fragment.Load(NewSourcePath, GL_FRAGMENT_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::FragmentSource(std::string_view NewSourceString)
{
    Fragment.Load(NewSourceString, GL_FRAGMENT_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::FragmentSource(const char* NewSourceString)
{
    Fragment.Load(std::string_view { NewSourceString } , GL_FRAGMENT_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::FragmentSource(const ShaderSource& InSource)
{
    Fragment = InSource;
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::ComputeSource(const Path& NewSourcePath)
{
    Compute.Load(NewSourcePath, GL_COMPUTE_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::ComputeSource(std::string_view NewSourceString)
{
    Compute.Load(NewSourceString, GL_COMPUTE_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::ComputeSource(const char* NewSourceString)
{
    Compute.Load(std::string_view { NewSourceString } , GL_COMPUTE_SHADER);
    return *this;
}

ShaderProgramBuilder& ShaderProgramBuilder::ComputeSource(const ShaderSource& InSource)
{
    Compute = InSource;
    return *this;
}

GLuint ShaderProgramBuilder::CompileShader(const ShaderSource& Source)
{
    const GLenum Type   = Source.GetType();
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