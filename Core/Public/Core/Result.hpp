#pragma once
#include "Assert.hpp"
#include "CoreMinimal.hpp"

namespace Oggle
{
    enum class EResultState : uint8
    {
        Empty,
        Valid,
        Error,
    };

    // A type-safe container which can hold either a value or an error type. Error type is a string by default, but can
    // be more customized. Defaults to an uninitialized, empty state unless a Value or Error is provided or set later.
    template<typename T, typename E = std::string>
    struct Result
    {
        Result();
        ~Result();

        Result           (const Result&);
        Result& operator=(const Result&);
        Result           (      Result&&);
        Result& operator=(      Result&&);

        // If T ane E aren't the same type, you can construct directly from values and errors
        Result(const T&) requires(!std::same_as<T, E>);
        Result(T&&)      requires(!std::same_as<T, E>);
        Result(const E&) requires(!std::same_as<T, E>);
        Result(E&&)      requires(!std::same_as<T, E>);

        // Coerces to bool
        explicit operator bool()  const { return IsValid(); }

        // Otherwise these factories are provided
        [[nodiscard]] static Result       FromValue(const T&);
        [[nodiscard]] static Result       FromValue(T&&);
        [[nodiscard]] static Result       FromError(const E&);
        [[nodiscard]] static Result       FromError(E&&);

        // Query the state of the Result
        [[nodiscard]]        EResultState GetState() const;
        [[nodiscard]]        bool         IsEmpty()  const;
        [[nodiscard]]        bool         IsValid()  const;
        [[nodiscard]]        bool         IsError()  const;

        // Retrieve Error. Requires IsError() to be true, asserts otherwise.
        [[nodiscard]]        E&           GetError();
        [[nodiscard]]        const E&     GetError() const;

        // Retrieve Value. Requires IsValid() to be true, asserts otherwise.
        [[nodiscard]]        T&           GetValue();
        [[nodiscard]]        const T&     GetValue() const;

        // Take ownership of Value. Requires IsValid() to be true, asserts otherwise.
        [[nodiscard]]        T&&          MoveValue();

        // Take ownership of Error. Requires IsError() to be true, asserts otherwise.
        [[nodiscard]]        E&&          MoveError();

        // If IsValid() is true, returns Value. Otherwise returns the default value provided.
        [[nodiscard]]        T&           GetValue(T&);
        [[nodiscard]]        const T&     GetValue(const T&);

        // If IsError() is true, returns Error. Otherwise returns the default error provided.
        [[nodiscard]]        E&           GetErrorOr(E&);
        [[nodiscard]]        const E&     GetErrorOr(const E&);

        // Set Value. (Destroys any Error which may be present)
                             void         SetValue(const T&);
                             void         SetValue(T&&);

        // Set Error. (Destroys any Value which may be present)
                             void         SetError(const E&);
                             void         SetError(E&&);

        // Return the Result to its default empty state. Destroys any stored Value or Error.
                             void         Clear();

    private:
        union ResultMemory
        {
            T Value;
            E Error;
            ResultMemory() {};
            ~ResultMemory() {};
        } Memory;

        EResultState State { EResultState::Empty };
    };

    template<typename T, typename E>
    T& Result<T, E>::GetValue(T& Default)
    {
        return IsValid() ? GetValue() : Default;
    }

    template<typename T, typename E>
    const T& Result<T, E>::GetValue(const T& Default)
    {
        return IsValid() ? GetValue() : Default;
    }

    template<typename T, typename E>
    E& Result<T, E>::GetErrorOr(E& Default)
    {
        return IsError() ? GetError() : Default;
    }

    template<typename T, typename E>
    const E& Result<T, E>::GetErrorOr(const E& Default)
    {
        return IsError() ? GetError() : Default;
    }

    template<typename T, typename E>
    void     Result<T, E>::SetValue(const T& InValue)
    {
        if (&Memory.Value == &InValue)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.Value, InValue);
        State = EResultState::Valid;
    }

    template<typename T, typename E>
    void Result<T, E>::SetValue(T&& InValue)
    {
        if (&Memory.Value == &InValue)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.Value, std::move(InValue));
        State = EResultState::Valid;
    }

    template<typename T, typename E>
    void Result<T, E>::SetError(const E& InError)
    {
        if (&Memory.Error == &InError)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.Error, InError);
        State = EResultState::Error;
    }

    template<typename T, typename E>
    void Result<T, E>::SetError(E&& InError)
    {
        if (&Memory.Error == &InError)
        {
            return;
        }

        Clear();
        std::construct_at(&Memory.Error, std::move(InError));
        State = EResultState::Error;
    }

template<typename T, typename E>
void     Result<T,E>::Clear()
{
    switch (State)
    {
        case EResultState::Empty:
            break;
        case EResultState::Valid:
            std::destroy_at(&Memory.Value);
            break;
        case EResultState::Error:
            std::destroy_at(&Memory.Error);
            break;
    }

    State = EResultState::Empty;
}

template<typename T, typename E>
Result<T, E>::Result()
{
    State = EResultState::Empty;
}

template<typename T, typename E>
Result<T, E>::~Result()
{
    Clear();
}

template<typename T, typename E>
Result<T, E>::Result(const T& InValue) requires(!std::same_as<T, E>)
{
    std::construct_at(&Memory.Value, InValue);
    State = EResultState::Valid;
}

template<typename T, typename E>
Result<T, E>::Result(T&& InValue) requires(!std::same_as<T, E>)
{
    std::construct_at(&Memory.Value, std::move(InValue));
    State = EResultState::Valid;
}

template<typename T, typename E>
Result<T, E>::Result(const E& InError) requires(!std::same_as<T, E>)
{
    std::construct_at(&Memory.Error, InError);
    State = EResultState::Error;
}

template<typename T, typename E>
Result<T, E>::Result(E&& InError) requires(!std::same_as<T, E>)
{
    std::construct_at(&Memory.Error, std::move(InError));
    State = EResultState::Error;
}

template<typename T, typename E>
Result<T, E> Result<T, E>::FromValue(const T& InValue)
{
    Result Res{};
    Res.SetValue(InValue);
    return Res;
}

template<typename T, typename E>
Result<T, E> Result<T, E>::FromValue(T&& InValue)
{
    Result Res{};
    Res.SetValue(std::move(InValue));
    return Res;
}

template<typename T, typename E>
Result<T, E> Result<T, E>::FromError(const E& InError)
{
    Result Res{};
    Res.SetError(InError);
    return Res;
}

template<typename T, typename E>
Result<T, E> Result<T, E>::FromError(E&& InError)
{
    Result Res{};
    Res.SetError(std::move(InError));
    return Res;
}

template<typename T, typename E>
EResultState Result<T, E>::GetState() const { return State; }

template<typename T, typename E>
bool Result<T, E>::IsEmpty() const { return State == EResultState::Empty; }

template<typename T, typename E>
bool Result<T, E>::IsValid() const { return State == EResultState::Valid; }

template<typename T, typename E>
bool Result<T, E>::IsError() const { return State == EResultState::Error; }

template<typename T, typename E>
E& Result<T, E>::GetError() { OGGLE_ASSERT(IsError()); return Memory.Error; }

template<typename T, typename E>
const E& Result<T, E>::GetError() const { OGGLE_ASSERT(IsError()); return Memory.Error; }

template<typename T, typename E>
T& Result<T, E>::GetValue() { OGGLE_ASSERT(IsValid()); return Memory.Value; }

template<typename T, typename E>
const T& Result<T, E>::GetValue() const { OGGLE_ASSERT(IsValid()); return Memory.Value; }

template<typename T, typename E>
T&& Result<T, E>::MoveValue() { OGGLE_ASSERT(IsValid()); return std::move(Memory.Value); }

template<typename T, typename E>
E&& Result<T, E>::MoveError() { OGGLE_ASSERT(IsError()); return std::move(Memory.Error); }

template<typename T, typename E>
Result<T,E>::Result(const Result& Other)
{
    switch (Other.State)
    {
        case EResultState::Empty:
            break;
        case EResultState::Valid:
            std::construct_at(&Memory.Value, Other.Memory.Value);
            break;
        case EResultState::Error:
            std::construct_at(&Memory.Error, Other.Memory.Error);
            break;
    }
    State = Other.State;
}

template<typename T, typename E>
Result<T, E>::Result(Result&& Other)
{
    switch (Other.State)
    {
        case EResultState::Empty:
            break;
        case EResultState::Valid:
            std::construct_at(&Memory.Value, std::move(Other.Memory.Value));
            break;
        case EResultState::Error:
            std::construct_at(&Memory.Error, std::move(Other.Memory.Error));
            break;
    }
    State = Other.State;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(const Result& Other)
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
            std::construct_at(&Memory.Value, Other.Memory.Value);
            break;
        case EResultState::Error:
            std::construct_at(&Memory.Error, Other.Memory.Error);
            break;
    }
    State = Other.State;

    return *this;
}

template<typename T, typename E>
Result<T, E>& Result<T, E>::operator=(Result&& Other)
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
            std::construct_at(&Memory.Value, std::move(Other.Memory.Value));
            break;
        case EResultState::Error:
            std::construct_at(&Memory.Error, std::move(Other.Memory.Error));
            break;
    }
    State = Other.State;

    return *this;
}
}
