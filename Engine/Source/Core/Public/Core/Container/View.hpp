#pragma once
#include "Core/Vocabulary/Optional.hpp"


namespace Oggle
{
    template<typename T>
    struct View
    {
        /// @brief Default constructor
        View();

        /// @brief Create a view from a pointer to the first element and the number of elements
        /// @param Start Pointer to element
        /// @param Count Number of elements
        View(T* Start, size_t Count);

        /// @brief Check if an index is within the bounds of the view
        /// @param Index Index to check
        /// @return boolean indicating whether Index corresponds to a valid element of the view
        bool IsValidIndex(size_t Index) const;

        /// @brief Subscript operator
        /// @param Index element to attempt to access
        /// @return An optional object which contains a reference to the element at Index if Index is valid, empty otherwise
        Optional<T&> operator[](size_t Index);

        /// @brief Subscript operator
        /// @param Index element to attempt to access
        /// @return An optional object which contains a const reference to the element at Index if Index is valid, empty otherwise
        Optional<const T&> operator[](size_t Index) const;

        using Iterator             =       T*;
        using ConstIterator        = const T*;
        using ReverseIterator      =       T*;
        using ConstReverseIterator = const T*;

        /// @brief Return iterator pointing to the first element
        /// @return
        Iterator begin()               { return m_Data; }

        /// @brief Return iterator pointing to the last element
        /// @return
        Iterator end()                 { return m_Data + m_Count; }

        /// @brief Return const iterator pointing to the first element
        /// @return
        ConstIterator cbegin()         { return m_Data; }

        /// @brief Return const iterator pointing to the last element
        /// @return
        ConstIterator cend()           { return m_Data + m_Count; }

        /// @brief Return iterator pointing to the last element
        /// @return
        ReverseIterator rbegin()       { return end() - 1; }

        /// @brief Return iterator pointing to the first element
        /// @return
        ReverseIterator rend()         { return m_Data - 1; };

        /// @brief Return const iterator pointing to the last element
        /// @return
        ConstReverseIterator crbegin() { return rbegin(); }

        /// @brief Return const iterator pointing to the first element
        /// @return
        ConstReverseIterator crend()   { return m_Data - 1; };

    private:
        T* m_Data;
        size_t m_Count;
    };

    template<typename T>
    View<T>::View(T* Start, size_t Count)
    {
        m_Data = Start;
        m_Count = Count;
    }

    template<typename T>
    bool View<T>::IsValidIndex(size_t Index) const
    {
        return Index < m_Count;
    }

    template<typename T>
    Optional<T&> View<T>::operator[](size_t Index)
    {
        if (IsValidIndex(Index))
        {
            return { m_Data[Index] };
        }
        return { };
    }
}