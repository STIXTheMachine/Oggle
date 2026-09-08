#pragma once
#include "Core/Vocabulary/Optional.hpp"

namespace Oggle
{
    template<typename T>
    struct Span
    {
        /// @brief Default constructor
        Span();

        /// @brief Create a view from a pointer to the first element and the number of elements
        /// @param Start Pointer to element
        /// @param Count Number of elements
        Span(T* Start, size_t Count);

        /// @brief Create a view from an existing static array
        template<size_t N>
        explicit Span(T (&)[N]);

        /// @brief Check if an index is within the bounds of the view
        /// @param Index Index to check
        /// @return boolean indicating whether Index corresponds to a valid element of the view
        [[nodiscard]] bool IsValidIndex(size_t Index) const;

        /// @brief Subscript operator
        /// @param Index element to attempt to access
        /// @return An optional object which contains a reference to the element at Index if Index is valid, empty otherwise
        [[nodiscard]] Optional<T&> operator[](size_t Index);

        /// @brief Subscript operator
        /// @param Index element to attempt to access
        /// @return An optional object which contains a const reference to the element at Index if Index is valid, empty otherwise
        [[nodiscard]] Optional<const T&> operator[](size_t Index) const;

        /// @brief Get the number of objects held by the Span
        /// @return Number of objects
        [[nodiscard]] size_t Count() const;

        /// @brief Get a raw pointer to the data held by the Span
        /// @return Pointer to first element
        [[nodiscard]] T* Data() const;

        using Iterator             =       T*;
        using ConstIterator        = const T*;
        // using ReverseIterator      =       T*;
        // using ConstReverseIterator = const T*;

        /// @brief Get an iterator pointing to the first element
        /// @return
        Iterator begin()               { return m_Data; }

        /// @brief Get an iterator pointing to the memory address after last element
        /// @return
        Iterator end()                 { return m_Data + m_Count; }

        /// @brief Get an const iterator pointing to the first element
        /// @return
        ConstIterator cbegin()         { return begin(); }

        /// @brief Get a const iterator pointing to the memory address after last element
        /// @return
        ConstIterator cend()           { return end(); }

    private:
        T* m_Data;
        size_t m_Count;
    };

    template<typename T>
    Span<T>::Span()
    {
        m_Data = nullptr;
        m_Count = 0;
    }

    template<typename T>
    Span<T>::Span(T* Start, size_t Count)
    {
        m_Data = Start;
        m_Count = Count;
    }

    template <typename T>
    template <size_t N>
    Span<T>::Span(T (&Array)[N])
    {
        m_Data = Array;
        m_Count = N;
    }

    template<typename T>
    bool Span<T>::IsValidIndex(size_t Index) const
    {
        return Index < m_Count;
    }

    template<typename T>
    Optional<T&> Span<T>::operator[](size_t Index)
    {
        if (IsValidIndex(Index))
        {
            return { m_Data[Index] };
        }
        return { };
    }

    template<typename T>
    Optional<const T&> Span<T>::operator[](size_t Index) const
    {
        if (IsValidIndex(Index))
        {
            return { m_Data[Index] };
        }
        return { };
    }

    template <typename T>
    size_t Span<T>::Count() const
    {
        return m_Count;
    }

    template <typename T>
    T* Span<T>::Data() const
    {
        return m_Data;
    }
}
