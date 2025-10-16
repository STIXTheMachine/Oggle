#pragma once
#include <type_traits>

// =====================================================================================================================
// TypesAreUnique
// =====================================================================================================================
template<typename... Ts>
struct TypesAreUnique;

template<>
struct TypesAreUnique<> : std::true_type {};

template<typename T>
struct TypesAreUnique<T> : std::true_type {};

/// Trait to determine if all types in a parameter pack are unique.
/// NOTE: qualifiers and refs are removed during the check. Foo, const Foo, const Foo&, etc are all equivalent here
template<typename First, typename... Rest>
struct TypesAreUnique<First, Rest...> : std::bool_constant<
        (!std::is_same_v<std::remove_cvref_t<First>, std::remove_cvref_t<Rest>> && ...)
        && TypesAreUnique<Rest...>::value
    > {};

/// Trait to determine if all types in a parameter pack are unique.
/// NOTE: qualifiers and refs are removed during the check. Foo, const Foo, const Foo&, etc are all equivalent here
template<typename... Ts>
constexpr bool TypesAreUnique_v = TypesAreUnique<Ts...>::value;

// =====================================================================================================================
// IsVertexAttribute
// =====================================================================================================================
template<typename T>
struct VertexAttribute;

template<typename T>
struct IsVertexAttribute : std::is_base_of<VertexAttribute<typename T::AttributeType>, T> {};

template<typename... Ts>
constexpr bool IsVertexAttribute_v = IsVertexAttribute<Ts...>::value;