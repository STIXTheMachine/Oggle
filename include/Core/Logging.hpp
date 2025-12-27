#pragma once
#include <string>
#include <string_view>
#include <format>
#include <Core/OggleType.hpp>
#include <Core/Bitflags.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define LOG_CATEGORY_STRINGIFY_IMPL(X) #X
#define LOG_CATEGORY_STRINGIFY(X) LOG_CATEGORY_STRINGIFY_IMPL(X)

#define LOG_CATEGORY_NAME_PASTE(Name) Log##Name
#define LOG_CATEGORY_NAME(Name) LOG_CATEGORY_NAME_PASTE(Name)
#define LOG_CATEGORY_NAME_STRING(Name) LOG_CATEGORY_STRINGIFY(LOG_CATEGORY_NAME(Name))

#define DECLARE_LOG_CATEGORY(Name, DefaultVerbosity, Method)                                                \
    struct LOG_CATEGORY_NAME(Name) final                                                                    \
    {                                                                                                       \
        LOG_CATEGORY_NAME(Name)() = delete;                                                                 \
        static consteval std::string_view GetCategoryName()     { return LOG_CATEGORY_NAME_STRING(Name); }; \
        using enum ELogVerbosity;                                                                           \
        static consteval ELogVerbosity    GetDefaultVerbosity() { return DefaultVerbosity; };               \
        using enum ELogMethod;                                                                              \
        static consteval ELogMethod       GetDefaultMethod()    { return Method; };                         \
        static inline    ELogVerbosity    Verbosity             { DefaultVerbosity };                       \
    };                                                                                                      \
    static_assert(CLogCategory<LOG_CATEGORY_NAME(Name)>);

enum class ELogMethod : uint8
{
    None       = 0,
    StdOut     = 1 << 0,    // Log to standard out
    StdErr     = 1 << 1,    // Log to standard error
    GlobalFile = 1 << 2,    // Log to the global log file
    OwnFile    = 1 << 3,    // Log to a category-specific log file
    Default    = StdOut | GlobalFile,
    All        = StdOut | StdErr | GlobalFile | OwnFile,
};
ENUM_FLAG_OPS(ELogMethod)

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

DECLARE_LOG_CATEGORY(Default, Info, StdOut)

// This needs to be set up differently
// Probably neeed to set up a global console/file sink and give LogCategories their own logger instances
// By default they can use the global sinks, but we'd be able to make it so that a category could log to only its own file,
// or the global file and its own etc
static inline struct GlobalLoggerInfo
{
    GlobalLoggerInfo()
        : ConsoleSink(std::make_shared<spdlog::sinks::stdout_color_sink_mt>())
        , FileSink(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Log.txt", true))
        , Logger(std::make_shared<spdlog::logger>(spdlog::logger("Logger", { ConsoleSink, FileSink })))
    {
        ConsoleSink->set_color(spdlog::level::info, ConsoleSink->yellow);
        // TODO: use pattern formatting to inject the [LogCategory] header rather than injecting it manually in LogImpl
        ConsoleSink->set_pattern("%^%v%$");
        FileSink->set_pattern("%v");
    }

    std::shared_ptr<spdlog::sinks::ansicolor_stdout_sink<spdlog::details::console_mutex> > ConsoleSink;
    std::shared_ptr<spdlog::sinks::basic_file_sink<std::mutex> > FileSink;
    std::shared_ptr<spdlog::logger> Logger;
} GLogger;

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

    GLogger.Logger->info("[{}] {}", Category::GetCategoryName(), FormattedLogString);

    if (Verbosity == Fatal)
    {
        std::abort();
    }
#endif
}

template<CLogCategory Category = LogDefault, typename... FormatArgs>
void Log(std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(Category::GetDefaultVerbosity(), Category::GetDefaultMethod(), FormatString, Args...);
};

template<CLogCategory Category = LogDefault, typename... FormatArgs>
void Log(ELogVerbosity OverrideVerbosity, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(OverrideVerbosity, Category::GetDefaultMethod(), FormatString, Args...);
};

template<CLogCategory Category = LogDefault, typename... FormatArgs>
void Log(ELogMethod OverrideMethod, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(Category::GetDefaultVerbosity(), OverrideMethod, FormatString, Args...);
};

template<CLogCategory Category = LogDefault, typename... FormatArgs>
void Log(ELogVerbosity OverrideVerbosity, ELogMethod OverrideMethod, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    LogImpl<Category>(OverrideVerbosity, OverrideMethod, FormatString, Args...);
};