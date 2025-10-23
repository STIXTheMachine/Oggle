//
// Created by jake on 10/22/25.
//
#pragma once

struct VirtualDestructor
{
    VirtualDestructor() = default;
    virtual ~VirtualDestructor() = default;
};

struct NonCopyable
{
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    void operator=(const NonCopyable&) = delete;
};

struct NonMovable
{
    NonMovable() = default;
    NonMovable(NonMovable&&) = delete;
    void operator=(NonMovable&&) = delete;
};