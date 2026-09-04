#pragma once

#include <format>

namespace Private // Oggle::Private
{

// Eventually this will be configurable, but for now they are just here so I can build the implementation around them.
static bool SuppressLogheaders() { return false; }
static bool SupporesLogHeaderCategories() { return false; }
static bool SuppresLogHeaderVerbosity() { return false; }

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


inline void InsertLogHeaders(std::string_view Header, std::string_view Message, std::string& Output)
{
    if (Message.empty())
    {
        return;
    }
    if (Header.empty())
    {
        Output = Message;
        return;
    }

    // Initial message header
    std::format_to(std::back_inserter(Output), "{}", Header);

    auto SegmentBegin = Message.begin();
    for (auto It = Message.begin(); It < Message.end(); ++It)
    {
        std::string_view Segment = { SegmentBegin, It };

        if (It + 1 == Message.end())
        {
            std::format_to(std::back_inserter(Output), "{}", std::string_view { SegmentBegin, Message.end() });
            break;
        }

        if (*It != '\n')
        {
            continue;
        }

        // It points to a newline. Send everything between this newline and the previous one to Output
        std::format_to(std::back_inserter(Output), "{}", Segment);

        // Point SegmentBegin to the next character, insert the newline and header
        SegmentBegin = It + 1;
        std::format_to(std::back_inserter(Output), "\n{}", Header);
    }
}

inline void DispatchToSinks(ELogSinks Sinks, BasicFileSink& CategoryFileSink, std::string_view Message)
{
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
        CategoryFileSink.WriteLn(Message);
    }
}

inline bool ShouldLog(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosity, ELogSinks Sinks)
{
    if (MessageVerbosity == ELogVerbosity::Fatal)
    {
        return true;
    }

    if (Sinks == ELogSinks::None || MessageVerbosity > CategoryVerbosity || MessageVerbosity > GMaxLogLevel())
    {
        return false;
    }

    return true;
}

template<CLogCategory Category>
void LogImpl(ELogVerbosity Verbosity, ELogSinks Sinks, std::string_view FormattedMessage_NoHeaders)
{
    if (!ShouldLog(Verbosity, Category::Verbosity, Sinks))
    {
        return;
    }

    std::string FullyFormattedMessage;

    if (!SuppressLogheaders()) [[likely]]
    {
        std::string Header;
        if (!SupporesLogHeaderCategories())
        {
            Header += std::format("[{}] ", Category::CategoryName);
        }
        if (!SuppresLogHeaderVerbosity())
        {
            Header += std::format("[{}] ", ParseToString(Category::Verbosity));
        }

        // One header for the beginning of the message plus one after every newline in the message itself
        const size_t NumHeaders = 1 + std::count(FormattedMessage_NoHeaders.begin(), FormattedMessage_NoHeaders.end(), '\n');
        const size_t HeadersTotalSize = Header.size() * NumHeaders;
        const size_t FullyFormattedMessageSize = FormattedMessage_NoHeaders.size() + HeadersTotalSize;

        FullyFormattedMessage.reserve(FullyFormattedMessageSize);

        InsertLogHeaders(Header, FormattedMessage_NoHeaders, FullyFormattedMessage);
    }
    else
    {
        FullyFormattedMessage = FormattedMessage_NoHeaders;
    }


    DispatchToSinks(Sinks, Category::GetLogFileSink(), FullyFormattedMessage);

    if (Verbosity ==  ELogVerbosity::Fatal)
    {
        GStdErrSink().Flush();
        GStdOutSink().Flush();
        GLogFileSink().Flush();
        Category::GetLogFileSink().Flush();
    }
}
} // namespace Private
// Now in namespace Oggle

#define LOG_CATEGORY_STRINGIFY_IMPL(X) #X
#define LOG_CATEGORY_STRINGIFY(X) LOG_CATEGORY_STRINGIFY_IMPL(X)

#define LOG_CATEGORY_NAME_PASTE(Name) Log##Name
#define LOG_CATEGORY_NAME(Name) LOG_CATEGORY_NAME_PASTE(Name)
#define LOG_CATEGORY_NAME_STRING(Name) LOG_CATEGORY_STRINGIFY(LOG_CATEGORY_NAME(Name))

#ifdef OGGLE_NO_LOGGING
#define LOG(...)
#define LOGFMT(...)
#define ARGS(...)
#else

#define FMT(...) std::format(__VA_ARGS__)

#define LOG_IMPL(Category, Verbosity, Sinks, LogString) \
    { \
        using enum Oggle::ELogVerbosity; \
        using enum Oggle::ELogSinks; \
        Oggle::Private::LogImpl<Category>(Verbosity, Sinks, LogString); \
    }

#define LOGNOFMT_1(LogString) \
    LOG_IMPL(LogDefault, LogDefault::DefaultVerbosity, LogDefault::DefaultSinks, std::format("{}", LogString))

#define LOGNOFMT_2(Category, LogString) \
    LOG_IMPL(LOG_CATEGORY_NAME(Category), LOG_CATEGORY_NAME(Category)::DefaultVerbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, LogString)

#define LOGNOFMT_3(Category, Verbosity, LogString) \
    LOG_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, LOG_CATEGORY_NAME(Category)::DefaultSinks, LogString)

#define LOGNOFMT_4(Category, Verbosity, Sinks, LogString) \
    LOG_IMPL(LOG_CATEGORY_NAME(Category), Verbosity, Sinks, LogString)

#define DISPATCH_LOGNOFMT(_1, _2, _3, _4, NUM_ARGS, ...) NUM_ARGS

#define LOG(...) \
    DISPATCH_LOGNOFMT(__VA_ARGS__, LOGNOFMT_4, LOGNOFMT_3, LOGNOFMT_2, LOGNOFMT_1)(__VA_ARGS__)
#endif

#define DECLARE_LOG_CATEGORY(InName, InDefaultVerbosity, InDefaultSinks) \
struct LOG_CATEGORY_NAME(InName) \
{ \
    LOG_CATEGORY_NAME(InName)() = delete; \
    static constexpr std::string_view                 CategoryName     { LOG_CATEGORY_NAME_STRING(InName) };\
    static constexpr Oggle::ELogVerbosity    DefaultVerbosity { Oggle::ELogVerbosity::InDefaultVerbosity }; \
    static constexpr Oggle::ELogSinks        DefaultSinks     { Oggle::ELogSinks::InDefaultSinks }; \
    static inline    Oggle::ELogVerbosity    Verbosity        { DefaultVerbosity }; \
    static           Oggle::BasicFileSink&   GetLogFileSink(); \
}; \
static_assert(Oggle::Private::CLogCategory<LOG_CATEGORY_NAME(InName)>);

#define DEFINE_LOG_CATEGORY(InName) \
Oggle::BasicFileSink& LOG_CATEGORY_NAME(InName)::GetLogFileSink() \
{ \
static Oggle::BasicFileSink Sink { LOG_CATEGORY_NAME_STRING(InName), LOG_CATEGORY_NAME_STRING(InName) }; \
return Sink; \
}

#define DECLARE_LOG_CATEGORY_NEW(CategoryName, CategoryDefaultMessageVerbosity, CategoryDefaultVerbosityMask, CategoryDefaultSinks) \
namespace LOG_CATEGORY_NAME(CategoryName) \
{ \
    inline constexpr std::string_view      Name                    = LOG_CATEGORY_NAME_STRING(CategoryName); \
    inline constexpr Oggle::ELogVerbosity  DefaultMessageVerbosity = Oggle::ELogVerbosity::CategoryDefaultMessageVerbosity; \
    inline constexpr Oggle::ELogVerbosity  DefaultVerbosityMask    = Oggle::ELogVerbosity::CategoryDefaultVerbosityMask; \
    inline constexpr Oggle::ELogSinks      DefaultSinks            = Oggle::ELogSinks::CategoryDefaultSinks; \
    \
    inline constinit Oggle::ELogVerbosity  VerbosityMask           = DefaultVerbosityMask; \
    inline constinit Oggle::ELogSinks      Sinks                   = DefaultSinks; \
    \
    inline void                  ResetMaxVerbosity()   { VerbosityMask = DefaultVerbosityMask; } \
    inline void                  ResetSinks()          { Sinks = DefaultSinks; } \
    inline Oggle::BasicFileSink& GetCategoryFileSink() { static Oggle::BasicFileSink Sink { LOG_CATEGORY_NAME_STRING(InName), LOG_CATEGORY_NAME_STRING(InName) }; return Sink; } \
};