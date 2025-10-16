#pragma once
#include "Core/TypeTraits.hpp"
#include "Core/Tuple.hpp"
#include "Core/MathTypes.hpp"

#define USE_TUPLE_SHENANIGANS false

#define DECLARE_VERTEX_ATTRIBUTE(Name, BaseType)            \
    struct Vertex##Name : VertexAttribute<BaseType> {};     \
    static_assert(sizeof(Vertex##Name) == sizeof(BaseType));

#if USE_TUPLE_SHENANIGANS
template<typename... Ts>
struct TupleIfMany;

template<typename... Ts>
    requires (sizeof...(Ts) > 1)
struct TupleIfMany<Ts...>
{
    using Type = std::tuple<Ts...>;
    using Type::Type;
};

template<typename T>
struct TupleIfMany<T>
{
    using Type = T;
    using Type::Type;
};

template<typename... Ts>
using TupleIfMany_t = TupleIfMany<Ts...>::Type;
#endif

// Empty base class that basically serves as a type tag
template<typename AttribType>
struct VertexAttribute
{
    using AttributeType = AttribType;
    AttribType Value;
};

DECLARE_VERTEX_ATTRIBUTE(Position, Vec3);
DECLARE_VERTEX_ATTRIBUTE(Normal,   Vec3);
DECLARE_VERTEX_ATTRIBUTE(Color,    Vec3);
DECLARE_VERTEX_ATTRIBUTE(TexCoord, Vec3);

template<typename... Ts>
using BaseType =
#if USE_TUPLE_SHENANIGANS
    TupleIfMany<Ts...>;
#else
        TTuple<Ts...>;
#endif

template<typename... AttributeTypes>
requires
    (IsVertexAttribute_v<AttributeTypes> && ...) &&
    TypesAreUnique_v<AttributeTypes...>
struct TVertex : BaseType<AttributeTypes...>
{
    using Base = BaseType<AttributeTypes...>;
    using Base::Base;

    template<typename AttributeType>
    AttributeType& GetAttribute() { return std::get<AttributeType>(*this); }

    template<typename AttributeType>
    const AttributeType& GetAttribute() const { return std::get<AttributeType>(*this); }

    template<typename AttributeType>
    decltype(AttributeType::Value)& Get() { return GetAttribute<AttributeType>().Value; };

    template<typename AttributeType>
    const decltype(AttributeType::Value)& Get() const { return GetAttribute<AttributeType>().Value; };

    // Forwarding constructor to preserve aggregate-style semantics without creating {{{nested tuple hell}}}
    template<AttributeTypes...>
    TVertex(AttributeTypes&&... args) : Base(std::forward<AttributeTypes>(args)...) {}
};

using Vertex_P   = TVertex<VertexPosition>;
using Vertex_PN  = TVertex<VertexPosition, VertexNormal>;
using Vertex_PC  = TVertex<VertexPosition, VertexColor>;
using Vertex_PNC = TVertex<VertexPosition, VertexNormal, VertexColor>;
using Vertex_PNT = TVertex<VertexPosition, VertexNormal, VertexTexCoord>;