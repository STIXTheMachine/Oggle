#pragma once

namespace Private // Oggle::Logging::Private
{

BasicStdOutSink& GStdOutSink();
BasicStdErrSink& GStdErrSink();
BasicFileSink&   GLogFileSink();

template<typename T>
concept CLogCategory = requires
{
    { T::CategoryName     } -> std::same_as<const std::string_view&>;
    { T::DefaultVerbosity } -> std::same_as<const ELogVerbosity&>;
    { T::DefaultSinks     } -> std::same_as<const ELogSinks&>;
    { T::Verbosity        } -> std::same_as<ELogVerbosity&>;
    { T::GetLogFileSink() } -> std::same_as<BasicFileSink&>;
};

template<CLogCategory Category, typename... FormatArgs>
void LogImpl(ELogVerbosity MessageVerbosity, ELogSinks Sinks, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    // If fatal, log to all sinks and abort
    if (MessageVerbosity == ELogVerbosity::Fatal)
    {
        const std::string LogHeader = std::format("[{}] ", Category::CategoryName);

        const size_t FormattedMessageSize = std::formatted_size(FormatString, std::forward<FormatArgs>(Args)...) + LogHeader.size();

        std::string Message;
        Message.reserve(FormattedMessageSize);
        Message.append(LogHeader);

        std::format_to(std::back_inserter(Message), FormatString, std::forward<FormatArgs>(Args)...);

        const size_t NumNewlines = std::count(Message.begin(), Message.end(), '\n');
        const size_t NewlineHeadersTotalSize = LogHeader.size() * NumNewlines;

        Message.reserve(FormattedMessageSize + NewlineHeadersTotalSize);

        for (size_t i = 0; i < Message.size(); ++i)
        {
            if (Message[i] == '\n')
            {
                Message.replace(i, 1, "\n" + LogHeader);
            }
        }

        GStdOutSink().WriteLn(Message, true);
        GStdErrSink().WriteLn(Message, true);
        GLogFileSink().WriteLn(Message, true);
        Category::GetLogFileSink().WriteLn(Message, true);
        std::abort();
        return;
    }

    // Determine if log message should be emitted
    if (MessageVerbosity > Category::Verbosity || MessageVerbosity > GMaxLogLevel() || Sinks == ELogSinks::None)
    {
        return;
    }

    const std::string LogHeader = std::format("[{}] ", Category::CategoryName);

    const size_t FormattedMessageSize = std::formatted_size(FormatString, std::forward<FormatArgs>(Args)...) + LogHeader.size();

    std::string Message;
    Message.reserve(FormattedMessageSize);
    Message.append(LogHeader);

    std::format_to(std::back_inserter(Message), FormatString, std::forward<FormatArgs>(Args)...);

    const size_t NumNewlines = std::count(Message.begin(), Message.end(), '\n');
    const size_t NewlineHeadersTotalSize = LogHeader.size() * NumNewlines;

    Message.reserve(FormattedMessageSize + NewlineHeadersTotalSize);

    for (size_t i = 0; i < Message.size(); ++i)
    {
        if (Message[i] == '\n')
        {
            Message.replace(i, 1, "\n" + LogHeader);
        }
    }

    // Dispatch to sinks
    if (HasAnyFlags(Sinks, ELogSinks::StdErr))
    {
        GStdErrSink().WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::StdOut))
    {
        GStdOutSink().WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::GlobalFile))
    {
        GLogFileSink().WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::OwnFile))
    {
        Category::GetLogFileSink().WriteLn(Message);
    }
}
} // namespace Private
// Now in namespace Oggle::Logging

#define LOG_CATEGORY_STRINGIFY_IMPL(X) #X
#define LOG_CATEGORY_STRINGIFY(X) LOG_CATEGORY_STRINGIFY_IMPL(X)

#define LOG_CATEGORY_NAME_PASTE(Name) Log##Name
#define LOG_CATEGORY_NAME(Name) LOG_CATEGORY_NAME_PASTE(Name)
#define LOG_CATEGORY_NAME_STRING(Name) LOG_CATEGORY_STRINGIFY(LOG_CATEGORY_NAME(Name))

#ifdef OGGLE_NO_LOGGING
#define LOG(...)
#define LOGFMT(...)
#else

#define LOGFMT_IMPL(Category, Verbosity, Sinks, FormatString, ...) \
    { \
        using enum Oggle::Logging::ELogVerbosity; \
        using enum Oggle::Logging::ELogSinks; \
        Oggle::Logging::Private::LogImpl<Category>(Verbosity, Sinks, FormatString __VA_OPT__(,) __VA_ARGS__); \
    }

#define DISPATCH_LOGFMT(_1, _2, _3, _4, _5, NUM_ARGS, ...) NUM_ARGS

#define LOGFMT_1(InvalidArg) \
    static_assert(false, "LOGFMT() requires at least two arguments")

#define LOGFMT_2(FormatString, ...) \
    LOGFMT_IMPL(LogDefault, LogDefault::DefaultVerbosity, LogDefault::DefaultSinks, FormatString, __VA_ARGS__)

#define LOGFMT_3(Category, FormatString, ...) \
    LOGFMT_IMPL(LOG_CATEGORY_NAME(Category), LOG_CATEGORY_NAME(Category)::DefaultVerbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, FormatString, __VA_ARGS__)

#define LOGFMT_4(Category, Verbosity, FormatString, ...) \
    LOGFMT_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, FormatString, __VA_ARGS__)

#define LOGFMT_5(Category, Verbosity, Sinks, FormatString, ...) \
    LOGFMT_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, Sinks, FormatString, __VA_ARGS__)

#define LOGFMT(...) DISPATCH_LOGFMT(__VA_ARGS__, LOGFMT_5, LOGFMT_4, LOGFMT_3, LOGFMT_2, LOGFMT_1)(__VA_ARGS__)

#define LOGNOFMT_IMPL(Category, Verbosity, Sinks, LogString) \
    { \
        using enum Oggle::Logging::ELogVerbosity; \
        using enum Oggle::Logging::ELogSinks; \
        Oggle::Logging::Private::LogImpl<Category>(Verbosity, Sinks, "{}", LogString); \
    }

#define LOGNOFMT_1(LogString) \
    LOGNOFMT_IMPL(LogDefault, LogDefault::DefaultVerbosity, LogDefault::DefaultSinks, LogString)

#define LOGNOFMT_2(Category, LogString) \
    LOGNOFMT_IMPL(LOG_CATEGORY_NAME(Category), LOG_CATEGORY_NAME(Category)::DefaultVerbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, LogString)

#define LOGNOFMT_3(Category, Verbosity, LogString) \
    LOGNOFMT_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, LogString)

#define LOGNOFMT_4(Category, Verbosity, Sinks, LogString) \
    LOGNOFMT_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, Sinks, LogString)

#define DISPATCH_LOGNOFMT(_1, _2, _3, _4, NUM_ARGS, ...) NUM_ARGS

#define LOG(...) \
    DISPATCH_LOGNOFMT(__VA_ARGS__, LOGNOFMT_4, LOGNOFMT_3, LOGNOFMT_2, LOGNOFMT_1)(__VA_ARGS__)
#endif

#define DECLARE_LOG_CATEGORY(InName, InDefaultVerbosity, InDefaultSinks) \
struct LOG_CATEGORY_NAME(InName) \
{ \
    LOG_CATEGORY_NAME(InName)() = delete; \
    using enum Oggle::Logging::ELogVerbosity; \
    using enum Oggle::Logging::ELogSinks; \
    static constexpr std::string_view                 CategoryName     { LOG_CATEGORY_NAME_STRING(InName) };\
    static constexpr Oggle::Logging::ELogVerbosity    DefaultVerbosity { InDefaultVerbosity }; \
    static constexpr Oggle::Logging::ELogSinks        DefaultSinks     { InDefaultSinks }; \
    static inline    Oggle::Logging::ELogVerbosity    Verbosity        { DefaultVerbosity }; \
    static           Oggle::Logging::BasicFileSink&   GetLogFileSink(); \
}; \
static_assert(Oggle::Logging::Private::CLogCategory<LOG_CATEGORY_NAME(InName)>);

#define DEFINE_LOG_CATEGORY(InName) \
Oggle::Logging::BasicFileSink& LOG_CATEGORY_NAME(InName)::GetLogFileSink() \
{ \
static Oggle::Logging::BasicFileSink Sink { LOG_CATEGORY_NAME_STRING(InName), LOG_CATEGORY_NAME_STRING(InName) }; \
return Sink; \
}
