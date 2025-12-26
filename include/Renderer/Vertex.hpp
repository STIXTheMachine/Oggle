#pragma once
#include "Core/TypeTraits.hpp"
#include "Core/Tuple.hpp"
#include "Renderer/VertexAttributes.hpp"

// Is anything in this file remotely necessary? Nope! Was it a fun learning experience to make? Yep!

template<typename... AttributeTypes>
struct TVertex;

// This looks much scarier than it is. A TVertex is just a fancified TTuple made from its composite attributes
// The exception is when there is a single attribute; see the specialization below for that.
template<typename... AttributeTypes>
requires
    (sizeof...(AttributeTypes) > 1) &&
    (IsVertexAttribute_v<AttributeTypes> && ...) &&
    TypesAreUnique_v<AttributeTypes...>
struct TVertex<AttributeTypes...> : TTuple<AttributeTypes...>
{
    using Base = TTuple<AttributeTypes...>;
    using Base::Base;

    static constexpr std::size_t AttributeCount = sizeof...(AttributeTypes);
    static constexpr std::size_t Stride = sizeof(TTuple<AttributeTypes...>);

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

// In the case of a single attribute, a TVertex IS the attribute, just with the API above tacked on for consistency
// Can be constructed exactly like its attribute can - i.e. TVertex<VertexPosition> Foo { 0.f, 0.f 0.f }; is valid.
template<typename SingleAttribute>
requires
    IsVertexAttribute_v<SingleAttribute>
struct TVertex<SingleAttribute> : SingleAttribute
{
    using Base = SingleAttribute;
    using Base::Base;

    static constexpr std::size_t AttributeCount = 1;
    static constexpr std::size_t Stride = sizeof(SingleAttribute);

    template<typename AttributeType>
    AttributeType& GetAttribute() { return *this; }

    template<typename AttributeType>
    const AttributeType& GetAttribute() const { return *this; }

    template<typename AttributeType>
    decltype(AttributeType::Value)& Get() { return this->Value; };

    template<typename AttributeType>
    const decltype(AttributeType::Value)& Get() const { return this->Value; };

    template<typename... Args>
    TVertex(Args&&... args) : Base { AttributeType { std::forward<Args>(args)... } } {};

private:
    using AttributeType = Base::AttributeType;
};

using Vertex_P   = TVertex<VertexPosition>;
using Vertex_PN  = TVertex<VertexPosition, VertexNormal>;
using Vertex_PC  = TVertex<VertexPosition, VertexColor>;
using Vertex_PNC = TVertex<VertexPosition, VertexNormal, VertexColor>;
using Vertex_PNT = TVertex<VertexPosition, VertexNormal, VertexTexCoord>;