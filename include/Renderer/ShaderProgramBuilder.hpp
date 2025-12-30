#pragma once
#include <Core/Logging/Logging.hpp>
#include <Renderer/GraphicsLibs.hpp>
#include <Core/TypeBases.hpp>
#include <filesystem>
#include <optional>

// Struct representing a shader's source code. Holds a copy of the source code in memory as well as the path of the file
// the source code was loaded from, if it was loaded from a file.
struct ShaderSource
{
    using Path = std::filesystem::path;
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

    [[nodiscard]] std::optional<Path> GetSourceFilePath() const { return SourceFile; };

    [[nodiscard]] std::string_view GetSourceString() const { return SourceString; };

    // Returns true if the object has a source file and/or string attached.
    [[nodiscard]] bool             HasSource() const { return bHasSource; };

private:
    static inline Path ShaderDirectory { "Assets/Shaders" };
    std::optional<Path> SourceFile   {};
    std::string         SourceString {};
    bool                bHasSource   { false };
    GLenum              Type         {};
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

    ShaderProgramBuilder& SetVertexSource(const Path&);
    ShaderProgramBuilder& SetVertexSource(std::string_view);
    ShaderProgramBuilder& SetVertexSource(const char*);
    ShaderProgramBuilder& SetVertexSource(const ShaderSource&);

    ShaderProgramBuilder& SetTessControlSource(const Path&);
    ShaderProgramBuilder& SetTessControlSource(std::string_view);
    ShaderProgramBuilder& SetTessControlSource(const char*);
    ShaderProgramBuilder& SetTessControlSource(const ShaderSource&);

    ShaderProgramBuilder& SetTessEvalSource(const Path&);
    ShaderProgramBuilder& SetTessEvalSource(std::string_view);
    ShaderProgramBuilder& SetTessEvalSource(const char*);
    ShaderProgramBuilder& SetTessEvalSource(const ShaderSource&);

    ShaderProgramBuilder& SetGeometrySource(const Path&);
    ShaderProgramBuilder& SetGeometrySource(std::string_view);
    ShaderProgramBuilder& SetGeometrySource(const char*);
    ShaderProgramBuilder& SetGeometrySource(const ShaderSource&);

    ShaderProgramBuilder& SetFragmentSource(const Path&);
    ShaderProgramBuilder& SetFragmentSource(std::string_view);
    ShaderProgramBuilder& SetFragmentSource(const char*);
    ShaderProgramBuilder& SetFragmentSource(const ShaderSource&);

    ShaderProgramBuilder& SetComputeSource(const Path&);
    ShaderProgramBuilder& SetComputeSource(std::string_view);
    ShaderProgramBuilder& SetComputeSource(const char*);
    ShaderProgramBuilder& SetComputeSource(const ShaderSource&);

    const ShaderSource& GetVertexSource()      { return Vertex; };
    const ShaderSource& GetTessControlSource() { return TessControl; };
    const ShaderSource& GetTessEvalSource()    { return TessEval; }
    const ShaderSource& GetGeometrySource()    { return Geometry; };
    const ShaderSource& GetFragmentSource()    { return Fragment; }
    const ShaderSource& GetComputeSource()     { return Compute; }

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

DECLARE_LOG_CATEGORY(ShaderCompile, Info, Default);