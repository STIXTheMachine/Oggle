#pragma once
#include "Core/TypeTraits.hpp"
#include "Core/Tuple.hpp"
#include "Core/MathTypes.hpp"

// Empty base class that basically serves as a type tag
struct VertexAttribute {};

// These cannot be type aliases because it breaks the constraint on TVertex requiring the attribute types to be unique
struct VertexPosition : Vec3, VertexAttribute {};
struct VertexNormal   : Vec3, VertexAttribute {};
struct VertexColor    : Vec4, VertexAttribute {};
struct VertexTexCoord : Vec2, VertexAttribute {};

template<typename... AttributeTypes>
requires
    AllVertexAttributes_v<AttributeTypes...> &&
    TypesAreUnique_v<AttributeTypes...>
struct TVertex : TTuple<AttributeTypes...>
{
    template<typename AttributeType>
    AttributeType& get() { return std::get<AttributeType>(*this); }

    template<typename AttributeType>
    const AttributeType& get() { return std::get<AttributeType>(*this); }

    // Forwarding constructor to preserve aggregate-style semantics without creating {{{nested tuple hell}}}
    template<AttributeTypes...>
    explicit TVertex(AttributeTypes&&... args) : Base(std::forward<AttributeTypes>(args)...) {}

private:
    using Base = TTuple<AttributeTypes...>;
    using Base::Base;
};

using Vertex_P    = TVertex<VertexPosition>;
using Vertex_PN   = TVertex<VertexPosition, VertexNormal>;
using Vertex_PC   = TVertex<VertexPosition, VertexColor>;
using Vertex_PNC  = TVertex<VertexPosition, VertexNormal, VertexColor>;
using Vertex_PNT  = TVertex<VertexPosition, VertexNormal, VertexTexCoord>;