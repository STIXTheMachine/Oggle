#pragma once
#include <utility>
#include "Core/Utilities/Assert.hpp"

namespace Oggle
{
    enum class EOptionalState
    {
        Empty,
        Valid
    };

    template<typename ValueType>
    struct Optional
    {
        Optional() = default;

        template<typename Arg>
        requires (!std::is_same_v<std::remove_cvref_t<Arg>, Optional> && std::constructible_from<ValueType, Arg>)
        Optional(Arg&&);

        template<typename... Args>
        requires (std::constructible_from<ValueType, Args...>)
        Optional(Args&&...);

        Optional(const Optional&);
        Optional& operator=(const Optional&);

        Optional(Optional&&);
        Optional& operator=(Optional&&);

        template<typename Arg>
        void SetValue(Arg&& Value);

        [[nodiscard]] const ValueType&  GetValue() const &;
        [[nodiscard]] ValueType&        GetValue() &;

        [[nodiscard]] const ValueType&&  GetValue() const &&;
        [[nodiscard]] ValueType&&        GetValue() &&;

        [[nodiscard]] bool              IsValid()  const;
        [[nodiscard]] bool              IsEmpty()  const;
        [[nodiscard]] explicit operator bool()     const;
        [[nodiscard]] EOptionalState    GetState() const;

                      void              Clear();

    private:
        ValueType InternalValue;

              ValueType& GetValueUnsafe();
        const ValueType& GetValueUnsafe() const;

        EOptionalState State = EOptionalState::Empty;
    };

    template<typename ValueType>
    template<typename Arg>
        requires (!std::is_same_v<std::remove_cvref_t<Arg>, Optional<ValueType>> && std::constructible_from<ValueType, Arg>)
    Optional<ValueType>::Optional(Arg&& Value)
    {
        InternalValue = ValueType { std::forward<Arg>(Value) };
        State = EOptionalState::Valid;
    }

    template<typename ValueType>
    template<typename ... Args>
        requires (std::constructible_from<ValueType, Args...>)
    Optional<ValueType>::Optional(Args&&... InArgs)
    {
        InternalValue = ValueType { std::forward<Args>(InArgs)... };
        State = EOptionalState::Valid;
    }

    template<typename ValueType>
    Optional<ValueType>::Optional(const Optional& Other)
    {
        State = Other.State;
        if (Other)
        {
            InternalValue = Other.InternalValue;
        }
    }

    template<typename ValueType>
    Optional<ValueType>& Optional<ValueType>::operator=(const Optional& Other)
    {
        if (*this == Other) return *this;

        State = Other.State;
        if (Other)
        {
            InternalValue = Other.InternalValue;
        }

        return *this;
    }

    template<typename ValueType>
    Optional<ValueType>::Optional(Optional&& Other)
    {
        State = Other.State;
        if (Other)
        {
            InternalValue = std::move(Other.InternalValue);
        }
    }

    template<typename ValueType>
    Optional<ValueType>& Optional<ValueType>::operator=(Optional&& Other)
    {
        if (*this == Other) return *this;

        State = Other.State;
        if (Other)
        {
            InternalValue = std::move(Other.InternalValue);
        }

        return *this;
    }

    template<typename ValueType>
    template<typename Arg>
    void Optional<ValueType>::SetValue(Arg&& Value)
    {
        InternalValue = std::forward<Arg>(Value);
        State = EOptionalState::Valid;
    }

    template<typename ValueType>
    const ValueType& Optional<ValueType>::GetValue() const &
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return InternalValue;
    }

    template<typename ValueType>
    ValueType& Optional<ValueType>::GetValue() &
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return InternalValue;
    }

    template<typename ValueType>
    const ValueType&& Optional<ValueType>::GetValue() const &&
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return std::move(InternalValue);
    }

    template<typename ValueType>
    ValueType&& Optional<ValueType>::GetValue() &&
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return std::move(InternalValue);
    }

    template<typename ValueType>
    bool Optional<ValueType>::IsValid() const
    {
        return State == EOptionalState::Valid;
    }

    template<typename ValueType>
    bool Optional<ValueType>::IsEmpty() const
    {
        return State == EOptionalState::Empty;
    }

    template<typename ValueType>
    Optional<ValueType>::operator bool() const
    {
        return IsValid();
    }

    template<typename ValueType>
    EOptionalState Optional<ValueType>::GetState() const
    {
        return State;
    }

    template<typename ValueType>
    void Optional<ValueType>::Clear()
    {

    }

    template<typename ValueType>
    ValueType& Optional<ValueType>::GetValueUnsafe()
    {
        return InternalValue;
    }

    template<typename ValueType>
    const ValueType& Optional<ValueType>::GetValueUnsafe() const
    {
        return InternalValue;
    }
}
