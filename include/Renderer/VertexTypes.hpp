#pragma once
#include "Core/TypeTraits.hpp"
#include "Core/Tuple.hpp"
#include "Core/MathTypes.hpp"

#define USE_TUPLE_SHENANIGANS false

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
DECLARE_VERTEX_ATTRIBUTE(Normal,   Vec3);
DECLARE_VERTEX_ATTRIBUTE(Color,    Vec3);
DECLARE_VERTEX_ATTRIBUTE(TexCoord, Vec3);

template<typename... AttributeTypes>
struct TVertex;

template<typename... AttributeTypes>
requires
    (sizeof...(AttributeTypes) > 1) &&
    (IsVertexAttribute_v<AttributeTypes> && ...) &&
    TypesAreUnique_v<AttributeTypes...>
struct TVertex<AttributeTypes...> : TTuple<AttributeTypes...>
{
    using Base = TTuple<AttributeTypes...>;
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

template<typename T, typename... Args>
concept AggregateConstructibleFrom = requires { T { std::declval<Args>()... }; };

template<typename SingleAttribute>
requires
    IsVertexAttribute_v<SingleAttribute>
struct TVertex<SingleAttribute> : SingleAttribute
{
    using Base = SingleAttribute;
    using Base::Base;

    template<typename AttributeType>
    AttributeType& GetAttribute() { return *this; }

    template<typename AttributeType>
    const AttributeType& GetAttribute() const { return *this; }

    template<typename AttributeType>
    decltype(AttributeType::Value)& Get() { return this->Value; };

    template<typename AttributeType>
    const decltype(AttributeType::Value)& Get() const { return this->Value; };
    template<typename... Args>
    TVertex(Args&&... args) : Base{ AttributeType { std::forward<Args>(args)... } } {};

private:
    using AttributeType = Base::AttributeType;
};

using Vertex_P   = TVertex<VertexPosition>;
using Vertex_PN  = TVertex<VertexPosition, VertexNormal>;
using Vertex_PC  = TVertex<VertexPosition, VertexColor>;
using Vertex_PNC = TVertex<VertexPosition, VertexNormal, VertexColor>;
using Vertex_PNT = TVertex<VertexPosition, VertexNormal, VertexTexCoord>;