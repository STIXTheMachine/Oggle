#pragma once
#include <string>
#include <string_view>
#include <format>
#include <print>
#include <Core/OggleType.hpp>

#define LOG_CATEGORY_STRINGIFY_IMPL(X) #X
#define LOG_CATEGORY_STRINGIFY(X) LOG_CATEGORY_STRINGIFY_IMPL(X)

#define LOG_CATEGORY_NAME_PASTE(Name) Name##Log
#define LOG_CATEGORY_NAME(Name) LOG_CATEGORY_NAME_PASTE(Name)
#define LOG_CATEGORY_NAME_STRING(Name) LOG_CATEGORY_STRINGIFY(LOG_CATEGORY_NAME(Name))

#define DECLARE_LOG_CATEGORY(Name, DefaultVerbosity, Method)                                                \
    struct LOG_CATEGORY_NAME(Name) final                                                                    \
    {                                                                                                       \
        LOG_CATEGORY_NAME(Name)() = delete;                                                                 \
        static consteval std::string_view GetCategoryName()     { return LOG_CATEGORY_NAME_STRING(Name); }; \
        static consteval ELogVerbosity    GetDefaultVerbosity() { return DefaultVerbosity; };               \
        static consteval ELogMethod       GetDefaultMethod()    { return Method; };                         \
        static inline    ELogVerbosity    Verbosity             { DefaultVerbosity };                       \
    };                                                                                                      \
    static_assert(CLogCategory<LOG_CATEGORY_NAME(Name)>);

enum class ELogMethod : uint8
{
    None    = 0,
    Console = 1 << 0,
    File    = 1 << 2,
    Default = Console,
    All     = Console | File,
};

constexpr std::underlying_type_t<ELogMethod> operator&(ELogMethod Lhs, ELogMethod Rhs)
{
    using T = std::underlying_type_t<ELogMethod>;
    return static_cast<T>(Lhs) & static_cast<T>(Rhs);
}

enum class ELogVerbosity : uint8
{
    Fatal       = 0,    // Always logs to Console and File, then crashes
    Error       = 1,
    Warning     = 2,
    Info        = 3,
    Verbose     = 4,
    VeryVerbose = 5,
    Never       = 127,    // Never logs anything, no matter what
};

constexpr std::strong_ordering operator<=>(ELogVerbosity Lhs, ELogVerbosity Rhs)
{
    using T = std::underlying_type_t<ELogVerbosity>;
    return static_cast<T>(Lhs) <=> static_cast<T>(Rhs);
};

template<typename T>
concept CLogCategory = requires
{
    { T::GetCategoryName()     } -> std::convertible_to<std::string_view>;
    { T::GetDefaultVerbosity() } -> std::same_as<ELogVerbosity>;
    { T::GetDefaultMethod()    } -> std::same_as<ELogMethod>;
    { T::Verbosity             } -> std::same_as<ELogVerbosity&>;
};

DECLARE_LOG_CATEGORY(Default, ELogVerbosity::Info, ELogMethod::Console)

template<CLogCategory Category, typename... FormatArgs>
void LogImpl(ELogVerbosity Verbosity, ELogMethod Method, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
#ifndef DISABLE_LOGGING
    using enum ELogVerbosity;
    using enum ELogMethod;

    if (Verbosity > Category::Verbosity)
    {
        return;
    }

    const std::string FormattedLogString = std::format(FormatString, std::forward<FormatArgs>(Args)...);

    const bool bLogToConsole = (Method & Console) != 0 || Verbosity == Fatal;
    const bool bLogToFile = (Method & File) != 0 || Verbosity == Fatal;

    if (bLogToConsole)
    {
        std::println("[{}] {}", Category::GetCategoryName(), FormattedLogString);
    }
    if (bLogToFile)
    {
        // TODO
        // Honestly should just stitch in spdlog and call it a day
    }

    if (Verbosity == Fatal)
    {
        std::abort();
    }
#endif
}

template<CLogCategory Category = DefaultLog, typename... FormatArgs>
void Log(std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(Category::GetDefaultVerbosity(), Category::GetDefaultMethod(), FormatString, Args...);
};

template<CLogCategory Category = DefaultLog, typename... FormatArgs>
void Log(ELogVerbosity OverrideVerbosity, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(OverrideVerbosity, Category::GetDefaultMethod(), FormatString, Args...);
};

template<CLogCategory Category = DefaultLog, typename... FormatArgs>
void Log(ELogMethod OverrideMethod, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(Category::GetDefaultVerbosity(), OverrideMethod, FormatString, Args...);
};

template<CLogCategory Category = DefaultLog, typename... FormatArgs>
void Log(ELogVerbosity OverrideVerbosity, ELogMethod OverrideMethod, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(OverrideVerbosity, OverrideMethod, FormatString, Args...);
};