#pragma once
#include <type_traits>
#include <utility>

namespace Oggle
{
namespace Private::CanaryImpl
{
    template <typename T>
    concept Movable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

    template <typename T>
    concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;
}

template <typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
struct CopyMoveCanary
{
    CopyMoveCanary() = default;

    template <typename Arg>
        requires(!std::same_as<std::remove_cvref_t<Arg>, CopyMoveCanary> && std::constructible_from<WrappedType, Arg>)
    CopyMoveCanary(Arg&&);

    operator WrappedType&();
    operator const WrappedType&() const;

    CopyMoveCanary(CopyMoveCanary&& Other);
    CopyMoveCanary& operator=(CopyMoveCanary&&);

    CopyMoveCanary(const CopyMoveCanary&);
    CopyMoveCanary& operator=(const CopyMoveCanary&);

    bool   HasBeenCopied()  const { return NumCopies != 0; }
    size_t NumTimesCopied() const { return NumCopies; }

    bool   HasBeenMoved()  const { return NumMoves != 0; }
    size_t NumTimesMoved() const { return NumMoves; }

    ~CopyMoveCanary() = default;

    WrappedType WrappedValue;

private:
    mutable size_t NumMoves  = 0;
    mutable size_t NumCopies = 0;
};


template <typename WrappedType>
    requires (Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
template <typename Arg>
    requires(!std::same_as<std::remove_cvref_t<Arg>, CopyMoveCanary<WrappedType>> && std::constructible_from<
                 WrappedType, Arg>)
CopyMoveCanary<WrappedType>::CopyMoveCanary(Arg&& InArg)
{
    WrappedValue = std::forward<Arg>(InArg);
}

template<typename WrappedType>
    requires (Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>::operator WrappedType&()
{
    return WrappedValue;
}

template<typename WrappedType>
    requires (Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>::operator const WrappedType&() const
{
    return WrappedValue;
}

template <typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>::CopyMoveCanary(CopyMoveCanary&& Other)
{
    WrappedValue = std::move(Other.WrappedValue);
    ++Other.NumMoves;
}

template <typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>& CopyMoveCanary<WrappedType>::operator=(CopyMoveCanary&& Other)
{
    WrappedValue = std::move(Other.WrappedValue);
    ++Other.NumMoves;
    return *this;
}

template <typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>::CopyMoveCanary(const CopyMoveCanary& Other)
{
    WrappedValue = Other.WrappedValue;
    ++Other.NumCopies;
}

template <typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
CopyMoveCanary<WrappedType>& CopyMoveCanary<WrappedType>::operator=(const CopyMoveCanary& Other)
{
    WrappedValue = Other.WrappedValue;
    ++Other.NumCopies;
    return *this;
}
}
