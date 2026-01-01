#include "../../include/Renderer/Mesh.hpp"

Mesh::Mesh() {
    CreateResourceObjects();
}

Mesh::~Mesh() {
    DeleteResourceObjects();
}

bool Mesh::LoadData(std::span<const VertexType> Vertices, std::span<const GLuint> Indices) {
    return LoadBinaryData(std::as_bytes(Vertices), Vertices.size(), std::as_bytes(Indices), Indices.size());
}

bool Mesh::LoadBinaryData(std::span<const std::byte> VertexData, GLsizei InVertexCount,
    std::span<const std::byte> IndexData, GLsizei InIndexCount) {
    if (!glIsBuffer(VBO) || !glIsBuffer(EBO))
    {
        return false;
    }

    VertexCount = InVertexCount;
    IndexCount  = InIndexCount;

    glNamedBufferStorage(VBO, VertexData.size_bytes(), VertexData.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(EBO, IndexData.size_bytes(), IndexData.data(), GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(VAO, 0, VBO,                           0, sizeof(VertexType)); // Position
    glVertexArrayVertexBuffer(VAO, 1, VBO, sizeof(VertexType::Position), sizeof(VertexType)); // Color

    // Position
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    // Color
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 4, GL_FLOAT, GL_TRUE, 0);
    glVertexArrayAttribBinding(VAO, 1, 1);

    glVertexArrayElementBuffer(VAO, EBO);

    return true;
}

void Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);

}

void Mesh::CreateResourceObjects() {
    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);
}

void Mesh::DeleteResourceObjects() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    VAO = 0;
    VBO = 0;
    EBO = 0;
}
