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

struct ShaderProgramBuilder : NonCopyable, NonMovable, VirtualDestructor
{
    // Returns true if a valid GLSL shader program could be compiled with the set of sources attached.
    virtual bool   IsValidConfiguration() = 0;

    // Compiles and links the attached sources into an OpenGL shader program.
    // Returns the name of the program if compiling and linking was successful, 0 otherwise.
    virtual GLuint Build() = 0;
};

struct GraphicsShaderProgramBuilder final : ShaderProgramBuilder
{
    using Path = std::filesystem::path;

    // Returns true if a valid GLSL shader program could be compiled with the set of sources attached.
    // E.g. checks that at least a vertex shader and fragment shader are attached, AND that if one type of tesselation
    // shader is attached, so is the other.
    // Does not validate that any of the attached sources actually compile or link to form a working program.
    bool IsValidConfiguration() override;

    //GLuint Build() override;

    GraphicsShaderProgramBuilder& SetVertexSource(const Path&);
    GraphicsShaderProgramBuilder& SetVertexSource(std::string_view);
    GraphicsShaderProgramBuilder& SetTessControlSource(const Path&);
    GraphicsShaderProgramBuilder& SetTessControlSource(std::string_view);
    GraphicsShaderProgramBuilder& SetTessEvalSource(const Path&);
    GraphicsShaderProgramBuilder& SetTessEvalSource(std::string_view);
    GraphicsShaderProgramBuilder& SetGeometrySource(const Path&);
    GraphicsShaderProgramBuilder& SetGeometrySource(std::string_view);
    GraphicsShaderProgramBuilder& SetFragmentSource(const Path&);
    GraphicsShaderProgramBuilder& SetFragmentSource(std::string_view);


private:
    ShaderSource VertexShaderSource;
    ShaderSource TessControlShaderSource;
    ShaderSource TessEvalShaderSource;
    ShaderSource GeometryShaderSource;
    ShaderSource FragmentShaderSource;
};

struct ComputeShaderProgramBuilder final : ShaderProgramBuilder
{
    using Path = std::filesystem::path;

    // Compiles and links the attached source into an OpenGL shader program. Returns the name of the program if
    // compiling/linking was successful, 0 otherwise.
    GLuint Build() override;

    // Returns true if a valid GLSL shader program could be compiled with the set of sources attached.
    // Does not validate whether the attached source compiles to form a working program.
    bool IsValidConfiguration() override;

    ComputeShaderProgramBuilder& SetSource(const Path&);
    ComputeShaderProgramBuilder& SetSource(std::string_view);
    ComputeShaderProgramBuilder& SetSource(const char*);

private:
    ShaderSource Source;
};