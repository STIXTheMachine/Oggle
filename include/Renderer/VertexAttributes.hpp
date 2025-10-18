#pragma once
#include <Core/MathTypes.hpp>

#define DECLARE_VERTEX_ATTRIBUTE(Name, BaseType)            \
struct Vertex##Name : VertexAttribute<BaseType> {};     \
static_assert(sizeof(Vertex##Name) == sizeof(BaseType));

// Empty base class that basically serves as a type tag
template<typename Type>
struct VertexAttribute
{
    using AttributeType = Type;
    Type Value;
};

DECLARE_VERTEX_ATTRIBUTE(Position, Vec3);
DECLARE_VERTEX_ATTRIBUTE(Tangent,  Vec3);
DECLARE_VERTEX_ATTRIBUTE(Normal,   Vec3);
DECLARE_VERTEX_ATTRIBUTE(Binormal, Vec4);
DECLARE_VERTEX_ATTRIBUTE(Color,    Vec3);
DECLARE_VERTEX_ATTRIBUTE(TexCoord, Vec3);
DECLARE_VERTEX_ATTRIBUTE(Weight,   float);
DECLARE_VERTEX_ATTRIBUTE(Index,    unsigned int);