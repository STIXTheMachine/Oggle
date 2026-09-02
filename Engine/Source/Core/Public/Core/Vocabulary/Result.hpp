#pragma once
#include "Core/Utilities/Assert.hpp"
#include "Core/Utilities/MemoryUtils.hpp"
#include <string>

namespace Oggle
{
    enum class EResultState : uint8
    {
        Empty, // Result's underlying memory is uninitialized
        Valid, // Result contains a Value
        Error, // Result contains an Error
    };

    // A type-safe container which can hold either a value or an error type.
    template <typename Value, typename Error = std::string>
    requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    struct Result
    {
        Result();
        ~Result();

        Result(const Result&);
        Result& operator=(const Result&);

        Result(Result&&);
        Result& operator=(Result&&);

        Result(const Value&);
        Result(Value&&);
        Result(const Error&);
        Result(Error&&);

        // Coerces to bool as a convenience
        explicit operator bool() const { return IsValid(); }

        // Query the state of the Result
        [[nodiscard]] EResultState GetState() const;
        [[nodiscard]] bool         IsEmpty()  const;
        [[nodiscard]] bool         IsValid()  const;
        [[nodiscard]] bool         IsError()  const;

        // Retrieve Value. Requires IsValid() to be true, asserts otherwise.
        [[nodiscard]]       Value& GetValue();
        [[nodiscard]] const Value& GetValue() const;

        // Returns Value if IsValid() is true. Otherwise returns the default value provided. Note that this results in a copy!
        [[nodiscard]] Value GetValueOr(Value);

        // Take ownership of Value. Requires IsValid() to be true, asserts otherwise. Equivalent to std::move(GetValue()).
        [[nodiscard]] Value TakeValue();

        // Retrieve Error. Requires IsError() to be true, asserts otherwise.
        [[nodiscard]]       Error& GetError();
        [[nodiscard]] const Error& GetError() const;

        // Returns Error if IsError() is true. Otherwise returns the default value provided. Note that this results in a copy!
        [[nodiscard]] Error GetErrorOr(Error);

        // Take ownership of Error. Requires IsError() to be true, asserts otherwise. Equivalent to std::move(GetError()).
        [[nodiscard]] Error TakeError();

        // Set Value. (Destroys any Error which may be present)
        void SetValue(const Value&);
        void SetValue(Value&&);

        // Set Error. (Destroys any Value which may be present)
        void SetError(const Error&);
        void SetError(Error&&);

        // Return the Result to its default empty state. Destroys any stored Value or Error.
        void Clear();

    private:
        union ResultMemory
        {
            Value InternalValue;
            Error InternalError;
            ResultMemory()  {}; // Required when Value or Error is not trivially constructible
            ~ResultMemory() {}; // Required when Value or Error is not trivially destructible
        } Memory;

              Value& GetValueUnchecked();
        const Value& GetValueUnchecked() const;
              Error& GetErrorUnchecked();
        const Error& GetErrorUnchecked() const;

        EResultState State{EResultState::Empty};
    };

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result()
    {
        State = EResultState::Empty;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::~Result()
    {
        Clear();
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(const Result& Other)
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

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>& Result<Value, Error>::operator=(const Result& Other)
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

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(Result&& Other)
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

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>& Result<Value, Error>::operator=(Result&& Other)
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

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(const Value& InValue)
    {
        std::construct_at(&Memory.InternalValue, InValue);
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(Value&& InValue)
    {
        std::construct_at(&Memory.InternalValue, std::move(InValue));
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(const Error& InError)
    {
        std::construct_at(&Memory.InternalError, InError);
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Result<Value, Error>::Result(Error&& InError)
    {
        std::construct_at(&Memory.InternalError, std::move(InError));
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    EResultState Result<Value, Error>::GetState() const { return State; }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    bool Result<Value, Error>::IsEmpty() const { return State == EResultState::Empty; }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    bool Result<Value, Error>::IsValid() const { return State == EResultState::Valid; }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    bool Result<Value, Error>::IsError() const { return State == EResultState::Error; }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Value& Result<Value, Error>::GetValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false");
        return GetValueUnchecked();
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    const Value& Result<Value, Error>::GetValue() const
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false");
        return GetValueUnchecked();
    }

    template<typename Value, typename Error> requires (!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Value Result<Value, Error>::GetValueOr(Value Default)
    {
        return IsValid() ? Memory.InternalValue : Default;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Value Result<Value, Error>::TakeValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::TakeValue() when IsValid() is false");
        return std::move(GetValueUnchecked());
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Error& Result<Value, Error>::GetError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false");
        return GetErrorUnchecked();
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    const Error& Result<Value, Error>::GetError() const
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false");
        return GetErrorUnchecked();
    }

    template<typename Value, typename Error> requires (!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Error Result<Value, Error>::GetErrorOr(Error Default)
    {
        return IsError() ? Memory.InternalError : Default;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Error Result<Value, Error>::TakeError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false");
        return std::move(GetErrorUnchecked());
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    void Result<Value, Error>::SetValue(const Value& InValue)
    {
        if (&Memory.InternalValue == &InValue)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalValue, InValue);
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    void Result<Value, Error>::SetValue(Value&& InValue)
    {
        if (&Memory.InternalValue == &InValue)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalValue, std::move(InValue));
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    void Result<Value, Error>::SetError(const Error& InError)
    {
        if (&Memory.InternalError == &InError)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalError, InError);
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    void Result<Value, Error>::SetError(Error&& InError)
    {
        if (&Memory.InternalError == &InError)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.InternalError, std::move(InError));
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    void Result<Value, Error>::Clear()
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

    template<typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Value& Result<Value, Error>::GetValueUnchecked() { return Memory.InternalValue; }

    template<typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    const Value& Result<Value, Error>::GetValueUnchecked() const { return Memory.InternalValue; }

    template<typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    Error& Result<Value, Error>::GetErrorUnchecked() { return Memory.InternalError; }

    template<typename Value, typename Error>
        requires(!std::same_as<std::remove_cvref<Value>, std::remove_cvref<Error>>)
    const Error& Result<Value, Error>::GetErrorUnchecked() const { return Memory.InternalError; }

}