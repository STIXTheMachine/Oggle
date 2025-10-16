#pragma once
#include <Core/TypeTraits.hpp>
#include <Core/MathTypes.hpp>

// These cannot be type aliases because it breaks the constraint on TVertex requiring the attribute types to be unique
struct VertexPosition : Vec3 {};
struct VertexNormal   : Vec3 {};
struct VertexColor    : Vec4 {};
struct VertexTexCoord : Vec2 {};

template<typename... AttributeTypes>
requires TypesAreUnique_v<AttributeTypes...>
struct TVertex : std::tuple<AttributeTypes...>
{
    template<typename AttributeType>
    AttributeType& get() { return std::get<AttributeType>(*this); }

    template<typename AttributeType>
    const AttributeType& get() { return std::get<AttributeType>(*this); }
};

using Vertex_P    = TVertex<VertexPosition>;
using Vertex_PC   = TVertex<VertexPosition, VertexColor>;
using Vertex_PN   = TVertex<VertexPosition, VertexNormal>;
using Vertex_PNC  = TVertex<VertexPosition, VertexNormal>;
using Vertex_PNT  = TVertex<VertexPosition, VertexNormal, VertexTexCoord>;