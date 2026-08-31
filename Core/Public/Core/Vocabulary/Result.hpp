#pragma once
#include "../Utilities/Assert.hpp"
#include "CoreMinimal.hpp"

namespace Oggle
{
    enum class EResultState : uint8
    {
        Empty,
        Valid,
        Error,
    };

    // A type-safe container which can hold either a value or an error type. Error type is a string by default, but can be more customized.
    // Has 3 states:
    // Empty: the underlying union is uninitialized, contains neither a valid Value or Error object.
    // Valid: contains a valid Value object
    // Error: contains a valid Error object
    // Note that any form of move out of this object will result in the moved-from object being put in the Empty state.
    // See move constructor/assignment, MoveValueInto, MoveErrorInto for more information.
    template <typename Value, typename Error = std::string>
    struct Result
    {
        Result();
        ~Result();

        Result(const Result&);
        Result& operator=(const Result&);

        // Note that moving will cause Clear() to be called on the moved-from Result!
        Result(Result&&);
        Result& operator=(Result&&);

        // If T ane E aren't the same type, you can construct directly from values and errors
        Result(const Value&) requires(!std::same_as<Value, Error>);
        Result(Value&&) requires(!std::same_as<Value, Error>);
        Result(const Error&) requires(!std::same_as<Value, Error>);
        Result(Error&&) requires(!std::same_as<Value, Error>);

        // Coerces to bool
        explicit operator bool() const { return IsValid(); }

        // Otherwise these factories are provided
        [[nodiscard]] static Result FromValue(const Value&);
        [[nodiscard]] static Result FromValue(Value&&);
        [[nodiscard]] static Result FromError(const Error&);
        [[nodiscard]] static Result FromError(Error&&);

        // Query the state of the Result
        [[nodiscard]] EResultState GetState() const;
        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] bool IsError() const;

        // Retrieve Error. Requires IsError() to be true, asserts otherwise.
        [[nodiscard]] Error& GetError();
        [[nodiscard]] const Error& GetError() const;

        // Retrieve Value. Requires IsValid() to be true, asserts otherwise.
        [[nodiscard]] Value& GetValue();
        [[nodiscard]] const Value& GetValue() const;

        // Take ownership of Value. Requires IsValid() to be true, asserts otherwise. Note that this will call Clear()!
        [[nodiscard]] Value MoveValue();

        // Take ownership of Error. Requires IsError() to be true, asserts otherwise. Note that this will call Clear()!
        [[nodiscard]] Error MoveError();

        // If IsValid() is true, returns Value. Otherwise returns the default value provided.
        [[nodiscard]] Value& GetValue(Value&);
        [[nodiscard]] const Value& GetValue(const Value&);

        // If IsError() is true, returns Error. Otherwise returns the default error provided.
        [[nodiscard]] Error& GetErrorOr(Error&);
        [[nodiscard]] const Error& GetErrorOr(const Error&);

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

            ResultMemory()
            {
            }; // Required in case Value or Error is not trivially constructible
            ~ResultMemory()
            {
            }; // Required in case Value or Error is not trivially destructible
        } Memory;

        EResultState State{EResultState::Empty};
    };

    template <typename Value, typename Error>
    Value& Result<Value, Error>::GetValue(Value& Default)
    {
        return IsValid() ? GetValue() : Default;
    }

    template <typename Value, typename Error>
    const Value& Result<Value, Error>::GetValue(const Value& Default)
    {
        return IsValid() ? GetValue() : Default;
    }

    template <typename Value, typename Error>
    Error& Result<Value, Error>::GetErrorOr(Error& Default)
    {
        return IsError() ? GetError() : Default;
    }

    template <typename Value, typename Error>
    const Error& Result<Value, Error>::GetErrorOr(const Error& Default)
    {
        return IsError() ? GetError() : Default;
    }

    template <typename Value, typename Error>
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

    template <typename Value, typename Error>
    Result<Value, Error>::Result()
    {
        State = EResultState::Empty;
    }

    template <typename Value, typename Error>
    Result<Value, Error>::~Result()
    {
        Clear();
    }

    template <typename Value, typename Error>
    Result<Value, Error>::Result(const Value& InValue) requires(!std::same_as<Value, Error>)
    {
        std::construct_at(&Memory.InternalValue, InValue);
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
    Result<Value, Error>::Result(Value&& InValue) requires(!std::same_as<Value, Error>)
    {
        std::construct_at(&Memory.InternalValue, std::move(InValue));
        State = EResultState::Valid;
    }

    template <typename Value, typename Error>
    Result<Value, Error>::Result(const Error& InError) requires(!std::same_as<Value, Error>)
    {
        std::construct_at(&Memory.InternalError, InError);
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
    Result<Value, Error>::Result(Error&& InError) requires(!std::same_as<Value, Error>)
    {
        std::construct_at(&Memory.InternalError, std::move(InError));
        State = EResultState::Error;
    }

    template <typename Value, typename Error>
    Result<Value, Error> Result<Value, Error>::FromValue(const Value& InValue)
    {
        Result Res{};
        Res.SetValue(InValue);
        return Res;
    }

    template <typename Value, typename Error>
    Result<Value, Error> Result<Value, Error>::FromValue(Value&& InValue)
    {
        Result Res{};
        Res.SetValue(std::move(InValue));
        return Res;
    }

    template <typename Value, typename Error>
    Result<Value, Error> Result<Value, Error>::FromError(const Error& InError)
    {
        Result Res{};
        Res.SetError(InError);
        return Res;
    }

    template <typename Value, typename Error>
    Result<Value, Error> Result<Value, Error>::FromError(Error&& InError)
    {
        Result Res{};
        Res.SetError(std::move(InError));
        return Res;
    }

    template <typename Value, typename Error>
    EResultState Result<Value, Error>::GetState() const { return State; }

    template <typename Value, typename Error>
    bool Result<Value, Error>::IsEmpty() const { return State == EResultState::Empty; }

    template <typename Value, typename Error>
    bool Result<Value, Error>::IsValid() const { return State == EResultState::Valid; }

    template <typename Value, typename Error>
    bool Result<Value, Error>::IsError() const { return State == EResultState::Error; }

    template <typename Value, typename Error>
    Error& Result<Value, Error>::GetError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false.");
        return Memory.InternalError;
    }

    template <typename Value, typename Error>
    const Error& Result<Value, Error>::GetError() const
    {
        OGGLE_ASSERT_MSG(IsError(), "Attempting to call Result::GetError() when IsError() is false.");
        return Memory.InternalError;
    }

    template <typename Value, typename Error>
    Value& Result<Value, Error>::GetValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false.");
        return Memory.InternalValue;
    }

    template <typename Value, typename Error>
    const Value& Result<Value, Error>::GetValue() const
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::GetValue() when IsValid() is false.");
        return Memory.InternalValue;
    }

    template <typename Value, typename Error>
    Value Result<Value, Error>::MoveValue()
    {
        OGGLE_ASSERT_MSG(IsValid(), "Attempting to call Result::MoveValue() when IsValid() is false.");
        auto Out = std::move(Memory.InternalValue);
        Clear();
        return Out;
    }

    template <typename Value, typename Error>
    Error Result<Value, Error>::MoveError()
    {
        OGGLE_ASSERT_MSG(IsError(), "Call to Result::MoveError() when IsError() is false.");
        auto Out = std::move(Memory.InternalError);
        Clear();
        return Out;
    }

    template <typename Value, typename Error>
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
        Other.Clear();
    }

    template <typename Value, typename Error>
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
        Other.Clear();

        return *this;
    }
}
