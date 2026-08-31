#pragma once

namespace Oggle
{

template<typename T>
struct Unique
{
    Unique() = default;
    explicit Unique(T*);

               Unique(Unique&&) noexcept;
    Unique& operator=(Unique&&) noexcept;

    [[nodiscard]]       bool IsValid() const { return Value != nullptr; }
    [[nodiscard]]       bool IsNull()  const { return Value == nullptr; }
    [[nodiscard]]       T*   Get()           { return Value; }
    [[nodiscard]] const T*   Get()     const { return Value; }

    explicit operator bool() const { return IsValid(); }
    T* operator->() { return Get(); }
    T& operator*() { return *Get(); }

    void Clear();

    Unique(const Unique&) = delete;
    Unique& operator=(const Unique&) = delete;

private:
    T* Value {};
};

template<typename T, typename... Args>
Unique<T> MakeUnique(Args&&... InArgs)
{
    return Unique<T>(new T(std::forward<Args>(InArgs)...));
}

template<typename T>
Unique<T>::Unique(T* InValue)
{
    Value = InValue;
}

template<typename T>
Unique<T>::Unique(Unique&& Other) noexcept
{
    Value = Other.Value;
    Other.Value = nullptr;
}

template<typename T>
Unique<T>& Unique<T>::operator=(Unique&& Other) noexcept
{
    if (this == &Other)
    {
        return *this;
    }

    Value = Other.Value;
    Other.Value = nullptr;

    return *this;
}

template<typename T>
void Unique<T>::Clear()
{
    if (Value)
    {
        std::destroy_at(Value);
        Value = nullptr;
    }
}
}
