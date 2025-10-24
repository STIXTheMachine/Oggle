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

    // Sets SourceFile to NewSourceFile and also streams the contents of NewSourceFile into SourceString.
                  void             SetFromFile(const std::filesystem::path& NewSourceFile);

    // Copies the contents of NewSourceString into SourceString. Also invalidates SourceFile.
    // Use SetFromFile if you want to set the source from a file.
                  void             SetFromString(std::string_view NewSourceString);

    // Clears the source file path and releases the source file string
                  void             Reset();

    [[nodiscard]] NullablePath     GetSourceFilePath() const { return SourceFile; };

    [[nodiscard]] std::string_view GetSourceString() const { return SourceString; };

    // Returns true if the object has a source file and/or string attached.
    [[nodiscard]] bool             HasSource() const { return bHasSource; };

private:
    // Optional to support setting the source string directly
    NullablePath SourceFile {};
    std::string  SourceString {};
    bool         bHasSource = false;
};

struct ShaderProgramBuilder final : NonCopyable, NonMovable
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

    ShaderProgramBuilder& SetTessControlSource(const Path&);
    ShaderProgramBuilder& SetTessControlSource(std::string_view);
    ShaderProgramBuilder& SetTessControlSource(const char*);

    ShaderProgramBuilder& SetTessEvalSource(const Path&);
    ShaderProgramBuilder& SetTessEvalSource(std::string_view);
    ShaderProgramBuilder& SetTessEvalSource(const char*);

    ShaderProgramBuilder& SetGeometrySource(const Path&);
    ShaderProgramBuilder& SetGeometrySource(std::string_view);
    ShaderProgramBuilder& SetGeometrySource(const char*);

    ShaderProgramBuilder& SetFragmentSource(const Path&);
    ShaderProgramBuilder& SetFragmentSource(std::string_view);
    ShaderProgramBuilder& SetFragmentSource(const char*);

    ShaderProgramBuilder& SetComputeSource(const Path&);
    ShaderProgramBuilder& SetComputeSource(std::string_view);
    ShaderProgramBuilder& SetComputeSource(const char*);

private:
    ShaderSource Vertex;
    ShaderSource TessControl;
    ShaderSource TessEval;
    ShaderSource Geometry;
    ShaderSource Fragment;
    ShaderSource Compute;

    static GLuint CompileShader(const ShaderSource&, GLenum);

    static void PrintInfoLog(GLuint);
};