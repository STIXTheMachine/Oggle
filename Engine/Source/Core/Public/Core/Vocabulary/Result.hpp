#pragma once
#include "Core/Utilities/Assert.hpp"
#include "Core/Utilities/MemoryUtils.hpp"
#include <string>
#include <type_traits>
#include <concepts>
#include <memory>

namespace Oggle
{
    enum class EResultState : uint8
    {
        Empty, // Result's underlying memory is uninitialized
        Valid, // Result contains a Value
        Error, // Result contains an Error
    };

    // A type-safe container which can hold either a value or an error type.
    template <typename ValueType, typename ErrorType = std::string>
    requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    struct Result
    {
        Result();
        ~Result();

        Result(const Result&);
        Result& operator=(const Result&);

        Result(Result&&);
        Result& operator=(Result&&);

        Result(const ValueType&);
        Result(ValueType&&);
        Result(const ErrorType&);
        Result(ErrorType&&);

        // Coerces to bool as a convenience
        explicit operator bool() const { return IsValid(); }

        template <typename Arg>
        requires(std::same_as<std::remove_cvref_t<Arg>, std::remove_cvref_t<ValueType>>)
        Result& operator=(Arg&&);

        template <typename Arg>
        requires(std::same_as<std::remove_cvref_t<Arg>, std::remove_cvref_t<ErrorType>>)
        Result& operator=(Arg&&);

        // Query the state of the Result
        [[nodiscard]] EResultState GetState() const;
        [[nodiscard]] bool         IsEmpty()  const;
        [[nodiscard]] bool         IsValid()  const;
        [[nodiscard]] bool         IsError()  const;

        // Retrieve Value. Requires IsValid() to be true, asserts otherwise.
        [[nodiscard]]       ValueType& GetValue();
        [[nodiscard]] const ValueType& GetValue() const;

        // Returns Value if IsValid() is true. Otherwise returns the default value provided. Note that this results in a copy!
        [[nodiscard]] ValueType GetValueOr(ValueType) const;

        // Take ownership of Value. Requires IsValid() to be true, asserts otherwise. Equivalent to std::move(GetValue()).
        [[nodiscard]] ValueType TakeValue();

        // Retrieve Error. Requires IsError() to be true, asserts otherwise.
        [[nodiscard]]       ErrorType& GetError();
        [[nodiscard]] const ErrorType& GetError() const;

        // Returns Error if IsError() is true. Otherwise returns the default value provided. Note that this results in a copy!
        [[nodiscard]] ErrorType GetErrorOr(ErrorType) const;

        // Take ownership of Error. Requires IsError() to be true, asserts otherwise. Equivalent to std::move(GetError()).
        [[nodiscard]] ErrorType TakeError();

        // Set Value. (Destroys any Error which may be present)
        template<typename ValueTypeFwd>
        void SetValue(ValueTypeFwd&&);

        // Set Error. (Destroys any Value which may be present)
        template<typename ErrorTypeFwd>
        void SetError(ErrorTypeFwd&&);

        // Return the Result to its default empty state. Destroys any stored Value or Error.
        void Clear();

    private:
        union ResultMemory
        {
            ValueType InternalValue;
            ErrorType InternalError;
            ResultMemory()  {}; // Required when Value or Error is not trivially constructible
            ~ResultMemory() {}; // Required when Value or Error is not trivially destructible
        } Memory;

              ValueType& GetValueUnchecked();
        const ValueType& GetValueUnchecked() const;
              ErrorType& GetErrorUnchecked();
        const ErrorType& GetErrorUnchecked() const;

        EResultState State{EResultState::Empty};
    };

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result()
    {
        State = EResultState::Empty;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::~Result()
    {
        Clear();
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(const Result& Other)
    {
        switch (Other.State)
        {
            case EResultState::Empty:
                break;
            case EResultState::Valid:
                std::construct_at(&Memory.InternalValue, Other.Memory.InternalValue);
                break;
            case EResultState::Error:
                std::construct_at(&Memory.InternalError, Other.Memory.InternalError);
                break;
        }
        State = Other.State;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>& Result<ValueType, ErrorType>::operator=(const Result& Other)
    {
        if (this == &Other)
        {
            return *this;
        }

        Clear();

        switch (Other.State)
        {
            case EResultState::Empty:
                break;
            case EResultState::Valid:
                std::construct_at(&Memory.InternalValue, Other.Memory.InternalValue);
                break;
            case EResultState::Error:
                std::construct_at(&Memory.InternalError, Other.Memory.InternalError);
                break;
        }
        State = Other.State;

        return *this;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(Result&& Other)
    {
        switch (Other.State)
        {
            case EResultState::Empty:
                break;
            case EResultState::Valid:
                std::construct_at(&Memory.InternalValue, std::move(Other.Memory.InternalValue));
                break;
            case EResultState::Error:
                std::construct_at(&Memory.InternalError, std::move(Other.Memory.InternalError));
                break;
        }
        State = Other.State;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>& Result<ValueType, ErrorType>::operator=(Result&& Other)
    {
        if (this == &Other)
        {
            return *this;
        }

        Clear();

        switch (Other.State)
        {
            case EResultState::Empty:
                break;
            case EResultState::Valid:
                std::construct_at(&Memory.InternalValue, std::move(Other.Memory.InternalValue));
                break;
            case EResultState::Error:
                std::construct_at(&Memory.InternalError, std::move(Other.Memory.InternalError));
                break;
        }
        State = Other.State;

        return *this;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(const ValueType& InValue)
    {
        std::construct_at(&Memory.InternalValue, InValue);
        State = EResultState::Valid;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(ValueType&& InValue)
    {
        std::construct_at(&Memory.InternalValue, std::move(InValue));
        State = EResultState::Valid;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(const ErrorType& InError)
    {
        std::construct_at(&Memory.InternalError, InError);
        State = EResultState::Error;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    Result<ValueType, ErrorType>::Result(ErrorType&& InError)
    {
        std::construct_at(&Memory.InternalError, std::move(InError));
        State = EResultState::Error;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    EResultState Result<ValueType, ErrorType>::GetState() const { return State; }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    bool Result<ValueType, ErrorType>::IsEmpty() const { return State == EResultState::Empty; }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    bool Result<ValueType, ErrorType>::IsValid() const { return State == EResultState::Valid; }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    bool Result<ValueType, ErrorType>::IsError() const { return State == EResultState::Error; }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ValueType& Result<ValueType, ErrorType>::GetValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false");
        return GetValueUnchecked();
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    const ValueType& Result<ValueType, ErrorType>::GetValue() const
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false");
        return GetValueUnchecked();
    }

    template<typename ValueType, typename ErrorType> requires (!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ValueType Result<ValueType, ErrorType>::GetValueOr(ValueType Default) const
    {
        return IsValid() ? Memory.InternalValue : std::move(Default);
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ValueType Result<ValueType, ErrorType>::TakeValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::TakeValue() when IsValid() is false");
        return std::move(GetValueUnchecked());
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ErrorType& Result<ValueType, ErrorType>::GetError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false");
        return GetErrorUnchecked();
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    const ErrorType& Result<ValueType, ErrorType>::GetError() const
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false");
        return GetErrorUnchecked();
    }

    template<typename ValueType, typename ErrorType> requires (!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ErrorType Result<ValueType, ErrorType>::GetErrorOr(ErrorType Default) const
    {
        return IsError() ? Memory.InternalError : std::move(Default);
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ErrorType Result<ValueType, ErrorType>::TakeError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::TakeError() when IsError() is false");
        return std::move(GetErrorUnchecked());
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    template<typename ValueTypeFwd>
    void Result<ValueType, ErrorType>::SetValue(ValueTypeFwd&& InValue)
    {
        if (SAME_ADDRESS(&Memory.InternalError, &InValue))
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalValue, std::forward<ValueTypeFwd>(InValue));
        State = EResultState::Valid;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    template<typename ErrorTypeFwd>
    void Result<ValueType, ErrorType>::SetError(ErrorTypeFwd&& InError)
    {
        if (SAME_ADDRESS(&Memory.InternalError, &InError))
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalError, std::forward<ErrorTypeFwd>(InError));
        State = EResultState::Error;
    }

    template <typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    void Result<ValueType, ErrorType>::Clear()
    {
        switch (State)
        {
            case EResultState::Empty:
                break;
            case EResultState::Valid:
                std::destroy_at(&Memory.InternalValue);
                break;
            case EResultState::Error:
                std::destroy_at(&Memory.InternalError);
                break;
        }

        State = EResultState::Empty;
    }

    template<typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ValueType& Result<ValueType, ErrorType>::GetValueUnchecked() { return Memory.InternalValue; }

    template<typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    const ValueType& Result<ValueType, ErrorType>::GetValueUnchecked() const { return Memory.InternalValue; }

    template<typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    ErrorType& Result<ValueType, ErrorType>::GetErrorUnchecked() { return Memory.InternalError; }

    template<typename ValueType, typename ErrorType>
        requires(!std::same_as<std::remove_cvref_t<ValueType>, std::remove_cvref_t<ErrorType>>)
    const ErrorType& Result<ValueType, ErrorType>::GetErrorUnchecked() const { return Memory.InternalError; }

}