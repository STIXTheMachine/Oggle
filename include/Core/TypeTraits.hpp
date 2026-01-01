#pragma once
#include <type_traits>

// =====================================================================================================================
// TypesAreSame
// =====================================================================================================================
template<typename... Ts>
struct TypesAreSame;

template<>
struct TypesAreSame<> : std::true_type {};

template<typename T>
struct TypesAreSame<T> : std::true_type {};

template<typename First, typename... Rest>
struct TypesAreSame<First, Rest...> : std::bool_constant<
    (std::is_same_v<std::remove_cvref_t<First>, std::remove_cvref_t<Rest>> && ...)
    && TypesAreSame<Rest...>::value
    > {};

template<typename... Ts>
constexpr bool TypesAreSame_v = TypesAreSame<Ts...>::value;

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

template<typename... Ts>
constexpr bool TypesAreUnique_v = TypesAreUnique<Ts...>::value;

// =====================================================================================================================
// ValuesAreUnique
// =====================================================================================================================

template<auto... Ts>
requires TypesAreSame_v<decltype(Ts)...>
struct ValuesAreUnique;

template<>
struct ValuesAreUnique<> : std::true_type {};

template<auto T>
struct ValuesAreUnique<T> : std::true_type {};

/// Variation of TypesAreUnique for non-type template parameters. Requires that all values in the pack be unique.
template<auto First, auto... Rest>
struct ValuesAreUnique<First, Rest...> : std::bool_constant<
        ((First != Rest) && ...)
        && ValuesAreUnique<Rest...>::value
    > {};


/// Variation of TypesAreUnique for non-type template parameters. Requires that all values in the pack be unique.
template<auto... Ts>
constexpr bool ValuesAreUnique_v = ValuesAreUnique<Ts...>::value;