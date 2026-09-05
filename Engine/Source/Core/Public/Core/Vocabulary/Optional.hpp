#pragma once
#include <utility>

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
        Optional(const ValueType& Value);
        Optional(ValueType&& Value);

        Optional(const Optional&);
        Optional& operator=(const Optional&);

        Optional(Optional&&);
        Optional& operator=(Optional&&);

        template<typename ValueTypeFwd>
        void SetValue(ValueTypeFwd&& Value);

        [[nodiscard]] const ValueType&  GetValue() const;
        [[nodiscard]] ValueType&        GetValue();

        [[nodiscard]] bool              IsValid()  const;
        [[nodiscard]] explicit operator bool()     const;
        [[nodiscard]] EOptionalState    GetState() const;

    private:
        ValueType InternalValue;

              ValueType& GetValueUnsafe();
        const ValueType& GetValueUnsafe() const;

        EOptionalState State = EOptionalState::Empty;
    };

    template<typename ValueType>
    Optional<ValueType>::Optional(const ValueType& Value) : InternalValue(Value), State(EOptionalState::Valid) {}

    template<typename ValueType>
    Optional<ValueType>::Optional(ValueType&& Value) : InternalValue(std::move(Value)), State(EOptionalState::Valid) {}

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
    template<typename ValueTypeFwd>
    void Optional<ValueType>::SetValue(ValueTypeFwd&& Value)
    {
        InternalValue = std::forward<ValueTypeFwd>(Value);
        State = EOptionalState::Valid;
    }

    template<typename ValueType>
    const ValueType& Optional<ValueType>::GetValue() const
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return InternalValue;
    }

    template<typename ValueType>
    ValueType& Optional<ValueType>::GetValue()
    {
        OGGLE_ASSERT_MSG(IsValid(),"Attempting to call Optional::GetValue() when IsValid() is false");
        return {};
    }

    template<typename ValueType>
    bool Optional<ValueType>::IsValid() const
    {
        return State == EOptionalState::Valid;
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
