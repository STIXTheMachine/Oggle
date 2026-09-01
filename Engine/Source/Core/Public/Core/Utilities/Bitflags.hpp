#pragma once
#include <type_traits>

#define ENUM_FLAG_OPS(Enum) \
    using Enum_t = std::underlying_type_t<Enum>; \
    inline constexpr Enum  operator&  (Enum  Lhs, Enum Rhs)  { return static_cast<Enum>(static_cast<Enum_t>(Lhs) & static_cast<Enum_t>(Rhs)); } \
    inline constexpr Enum& operator&= (Enum& Lhs, Enum Rhs)  { Lhs = Lhs & Rhs; return Lhs; }; \
    inline constexpr Enum  operator|  (Enum  Lhs, Enum Rhs)  { return static_cast<Enum>(static_cast<Enum_t>(Lhs) | static_cast<Enum_t>(Rhs)); } \
    inline constexpr Enum& operator|= (Enum& Lhs, Enum Rhs)  { Lhs = Lhs | Rhs; return Lhs; }; \
    inline constexpr Enum  operator^  (Enum  Lhs, Enum Rhs)  { return static_cast<Enum>(static_cast<Enum_t>(Lhs) ^ static_cast<Enum_t>(Rhs)); } \
    inline constexpr Enum& operator^= (Enum& Lhs, Enum Rhs)  { Lhs = Lhs ^ Rhs; return Lhs; }; \
    inline constexpr Enum  operator~  (Enum  Arg)            { return static_cast<Enum>(~static_cast<Enum_t>(Arg)); } \
    inline constexpr bool  HasAnyFlags(Enum Arg, Enum Flags) { return (static_cast<Enum_t>(Arg) & static_cast<Enum_t>(Flags)) != 0; } \
    inline constexpr bool  HasAllFlags(Enum Arg, Enum Flags) { return (Arg & Flags) == Flags; }
