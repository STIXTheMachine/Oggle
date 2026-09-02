#pragma once
#include <type_traits>
#include <utility>

namespace Oggle
{
    namespace Private::CanaryImpl
    {
        template<typename T>
        concept Movable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

        template<typename T>
        concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;
    }

    template<typename WrappedType>
    requires(Private::CanaryImpl::Movable<WrappedType>)
    struct MoveCanary
    {
        explicit MoveCanary(WrappedType);

        MoveCanary(MoveCanary&& Other) noexcept;
        MoveCanary& operator=(MoveCanary&&) noexcept;

        MoveCanary(const MoveCanary&) = default;
        MoveCanary& operator=(const MoveCanary&) = default;

        ~MoveCanary() = default;

        WrappedType    WrappedValue;

        mutable bool   bMoved   = false;
        mutable size_t NumMoves = 0;
    };

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType>)
    MoveCanary<WrappedType>::MoveCanary(WrappedType InValue) : WrappedValue(InValue) {}

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType>)
    MoveCanary<WrappedType>::MoveCanary(MoveCanary&& Other) noexcept
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMoved = true;
        ++Other.NumMoves;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType>)
    MoveCanary<WrappedType>& MoveCanary<WrappedType>::operator=(MoveCanary&& Other) noexcept
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMoved = true;
        ++Other.NumMoves;
        return *this;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Copyable<WrappedType>)
    struct CopyCanary
    {
        CopyCanary(WrappedType);

        CopyCanary(CopyCanary&& Other) = default;
        CopyCanary& operator=(CopyCanary&&) = default;

        CopyCanary(const CopyCanary&);
        CopyCanary& operator=(const CopyCanary&);

        ~CopyCanary() = default;

        WrappedType WrappedValue;

        mutable bool   bCopied   = false;
        mutable size_t NumCopies = 0;
    };

    template<typename WrappedType>
	requires(Private::CanaryImpl::Copyable<WrappedType>)
    CopyCanary<WrappedType>::CopyCanary(WrappedType InValue) : WrappedValue(InValue) {}

    template<typename WrappedType>
	requires(Private::CanaryImpl::Copyable<WrappedType>)
    CopyCanary<WrappedType>::CopyCanary(const CopyCanary& Other)
    {
        WrappedValue = Other.WrappedValue;
        Other.bCopied = true;
        ++Other.NumCopies;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Copyable<WrappedType>)
    CopyCanary<WrappedType>& CopyCanary<WrappedType>::operator=(const CopyCanary& Other)
    {
        WrappedValue = Other.WrappedValue;
        Other.bCopied = true;
        ++Other.NumCopies;
        return *this;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    struct MoveCopyCanary
    {
        explicit MoveCopyCanary(WrappedType);

        MoveCopyCanary(MoveCopyCanary&& Other) noexcept;
        MoveCopyCanary& operator=(MoveCopyCanary&&) noexcept;

        MoveCopyCanary(const MoveCopyCanary&);
        MoveCopyCanary& operator=(const MoveCopyCanary&);

        ~MoveCopyCanary() = default;

        WrappedType WrappedValue;

        mutable bool   bMoved    = false;
        mutable bool   bCopied   = false;

        mutable size_t NumMoves  = 0;
        mutable size_t NumCopies = 0;
    };

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    MoveCopyCanary<WrappedType>::MoveCopyCanary(WrappedType InValue) : WrappedValue(InValue) {}

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    MoveCopyCanary<WrappedType>::MoveCopyCanary(MoveCopyCanary&& Other) noexcept
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMoved = true;
        ++Other.NumMoves;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    MoveCopyCanary<WrappedType>& MoveCopyCanary<WrappedType>::operator=(MoveCopyCanary&& Other) noexcept
    {
        WrappedValue = std::move(Other.WrappedValue);
        Other.bMoved = true;
        ++Other.NumMoves;
        return *this;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    MoveCopyCanary<WrappedType>::MoveCopyCanary(const MoveCopyCanary& Other)
    {
        WrappedValue = Other.WrappedValue;
        Other.bCopied = true;
        ++Other.NumCopies;
    }

    template<typename WrappedType>
	requires(Private::CanaryImpl::Movable<WrappedType> && Private::CanaryImpl::Copyable<WrappedType>)
    MoveCopyCanary<WrappedType>& MoveCopyCanary<WrappedType>::operator=(const MoveCopyCanary& Other)
    {
        WrappedValue = Other.WrappedValue;
        Other.bCopied = true;
        ++Other.NumCopies;
        return *this;
    }
}
