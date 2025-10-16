#pragma once
#include <type_traits>

// =====================================================================================================================
// TypesAreUnique
// =====================================================================================================================
template<typename... Types>
struct TypesAreUnique;

template<>
struct TypesAreUnique<> : std::true_type {};

template<typename Type>
struct TypesAreUnique<Type> : std::true_type {};

/// Trait to determine if all types in a parameter pack are unique.
/// NOTE: qualifiers and refs are removed during the check. Foo, const Foo, const Foo&, etc are all equivalent here
template<typename First, typename... Rest>
struct TypesAreUnique<First, Rest...> : std::bool_constant<
        (!std::is_same_v<std::remove_cvref_t<First>, std::remove_cvref_t<Rest>> && ...)
        && TypesAreUnique<Rest...>::value
    > {};

/// Trait to determine if all types in a parameter pack are unique.
/// NOTE: qualifiers and refs are removed during the check. Foo, const Foo, const Foo&, etc are all equivalent here
template<typename... Types>
constexpr bool TypesAreUnique_v = TypesAreUnique<Types...>::value;