#pragma once
#include <type_traits>
#include <utility>

namespace Oggle
{
    template<typename WrappedType>
    requires(std::is_move_constructible_v<WrappedType> && std::is_move_assignable_v<WrappedType>)
    struct MovedFromCanary
    {
        MovedFromCanary(WrappedType);

        MovedFromCanary(MovedFromCanary&& Other);
        MovedFromCanary& operator=(MovedFromCanary&&);

        MovedFromCanary(const MovedFromCanary&) = delete;
        MovedFromCanary& operator=(const MovedFromCanary&) = delete;

        ~MovedFromCanary() = default;

        WrappedType WrappedValue;
        bool bMovedFrom = false;
    };

    template<typename WrappedType> requires (std::is_move_constructible_v<WrappedType> && std::is_move_assignable_v<
        WrappedType>)
    MovedFromCanary<WrappedType>::MovedFromCanary(WrappedType InValue) : WrappedValue(InValue) {}

    template<typename WrappedType> requires (std::is_move_constructible_v<WrappedType> && std::is_move_assignable_v<
        WrappedType>)
    MovedFromCanary<WrappedType>::MovedFromCanary(MovedFromCanary&& Other)
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMovedFrom = true;
    }

    template<typename WrappedType> requires (std::is_move_constructible_v<WrappedType> && std::is_move_assignable_v<
        WrappedType>)
    MovedFromCanary<WrappedType>& MovedFromCanary<WrappedType>::operator=(MovedFromCanary&& Other)
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMovedFrom = true;
    }
}
