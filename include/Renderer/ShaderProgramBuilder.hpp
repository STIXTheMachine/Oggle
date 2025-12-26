#pragma once
#include <Renderer/GraphicsLibs.hpp>
#include <Core/TypeBases.hpp>
#include <filesystem>
#include <optional>

// Struct representing a shader's source code. Holds a copy of the source code in memory as well as the path of the file
// the source code was loaded from, if it was loaded from a file.
struct ShaderSource
{
    using NullablePath = std::optional<std::filesystem::path>;
    ShaderSource() = default;
    explicit ShaderSource(GLenum InType) : Type(InType) {};

    // Sets SourceFile to NewSourceFile and also streams the contents of NewSourceFile into SourceString.
                  void             Load(const std::filesystem::path& NewSourceFile, GLenum Type);

    // Copies the contents of NewSourceString into SourceString. Also invalidates SourceFile.
    // Use SetFromFile if you want to set the source from a file.
                  void             Load(std::string_view NewSourceString, GLenum Type);

    // Clears the source file path and releases the source file string
                  void             Reset();

    [[nodiscard]] GLenum           GetType() const { return Type; };

    [[nodiscard]] NullablePath     GetSourceFilePath() const { return SourceFile; };

    [[nodiscard]] std::string_view GetSourceString() const { return SourceString; };

    // Returns true if the object has a source file and/or string attached.
    [[nodiscard]] bool             HasSource() const { return bHasSource; };

private:
    // Optional to support setting the source string directly
    NullablePath SourceFile   {};
    std::string  SourceString {};
    bool         bHasSource   { false };
    GLenum       Type         {};
};

struct ShaderProgramBuilder final : NonCopyable
{
    using Path = std::filesystem::path;

    // Returns true if a valid GLSL shader program could be compiled with the set of sources attached.
    // Does not validate that any of the attached sources actually compile or link to form a working program.
    bool IsValidConfiguration() const;

    // Compiles and links attaached sources into a program and returns the name of the resulting program.
    // Returns name 0 if compilation or linking failed.
    GLuint Build();

    // Resets all attached sources to prepare for building a new program.
    void Reset();

    ShaderProgramBuilder& VertexSource(const Path&);
    ShaderProgramBuilder& VertexSource(std::string_view);
    ShaderProgramBuilder& VertexSource(const char*);
    ShaderProgramBuilder& VertexSource(const ShaderSource&);

    ShaderProgramBuilder& TessControlSource(const Path&);
    ShaderProgramBuilder& TessControlSource(std::string_view);
    ShaderProgramBuilder& TessControlSource(const char*);
    ShaderProgramBuilder& TessControlSource(const ShaderSource&);

    ShaderProgramBuilder& TessEvalSource(const Path&);
    ShaderProgramBuilder& TessEvalSource(std::string_view);
    ShaderProgramBuilder& TessEvalSource(const char*);
    ShaderProgramBuilder& TessEvalSource(const ShaderSource&);

    ShaderProgramBuilder& GeometrySource(const Path&);
    ShaderProgramBuilder& GeometrySource(std::string_view);
    ShaderProgramBuilder& GeometrySource(const char*);
    ShaderProgramBuilder& GeometrySource(const ShaderSource&);

    ShaderProgramBuilder& FragmentSource(const Path&);
    ShaderProgramBuilder& FragmentSource(std::string_view);
    ShaderProgramBuilder& FragmentSource(const char*);
    ShaderProgramBuilder& FragmentSource(const ShaderSource&);

    ShaderProgramBuilder& ComputeSource(const Path&);
    ShaderProgramBuilder& ComputeSource(std::string_view);
    ShaderProgramBuilder& ComputeSource(const char*);
    ShaderProgramBuilder& ComputeSource(const ShaderSource&);

private:
    ShaderSource Vertex      { GL_VERTEX_SHADER };
    ShaderSource TessControl { GL_TESS_CONTROL_SHADER };
    ShaderSource TessEval    { GL_TESS_EVALUATION_SHADER };
    ShaderSource Geometry    { GL_GEOMETRY_SHADER };
    ShaderSource Fragment    { GL_FRAGMENT_SHADER };
    ShaderSource Compute     { GL_COMPUTE_SHADER };

    static GLuint CompileShader(const ShaderSource&);

    static void PrintInfoLog(GLuint);
};