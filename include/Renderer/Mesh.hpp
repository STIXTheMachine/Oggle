#pragma once
#include <Renderer/Vertex.hpp>
#include <span>

    struct Mesh
    {
        using VertexType = Vertex_PC;

        Mesh();

        ~Mesh();

        bool LoadData(std::span<const VertexType> Vertices, std::span<const GLuint> Indices);

        bool LoadBinaryData(std::span<const std::byte> VertexData, GLsizei InVertexCount, std::span<const std::byte> IndexData, GLsizei InIndexCount);

        void Draw();

    private:

        void CreateResourceObjects();

        void DeleteResourceObjects();

        GLsizei VertexCount {};
        GLsizei IndexCount {};
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };
