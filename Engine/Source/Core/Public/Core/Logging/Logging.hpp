#pragma once
#include <utility>
#include <Core/Utilities/Bitflags.hpp>
#include <Core/Vocabulary/OggleType.hpp>
#include <Core/Logging/Sinks.hpp>
#include <format>

namespace Oggle
{
/// Specifies the destinations (sinks) for log messages.
///
/// Each log message can be sent to one or more sinks, which control where the message is recorded or displayed.
/// These values can be combined using bitwise OR (`|`) because each sink represents a separate bit.
///
/// Example: ELogSinks MySinks = ELogSinks::StdOut | ELogSinks::GlobalFile;
/// This will send the message to both the standard output console and the global log file.
///
/// Sinks are independent of verbosity; a message must pass verbosity filtering before being dispatched to any sinks.
///
/// See also: GStdOut, GStdErr, GLogFile.
enum class ELogSinks : uint8
{
    None       = 0,      // Do not log anywhere
    StdOut     = 1 << 0, // Log to GStdOut
    StdErr     = 1 << 1, // Log to GStdErr
    GlobalFile = 1 << 2, // Log to GLogFile
    OwnFile    = 1 << 3, // Log to the category-specific log file
    Default    = StdOut | GlobalFile | OwnFile,
    All        = StdOut | StdErr | GlobalFile | OwnFile,
};
ENUM_FLAG_OPS(ELogSinks)

/// Defines the verbosity levels for logging messages in a category.
///
/// Each log category has:
/// - A default message verbosity, which is used if a verbosity is not specified when logging a message
/// - A current verbosity mask
/// - 0-4 enabled sinks. (See ELogSink above.)
///
/// If the logged message's verbosity is greater than the category's verbosity mask, it is filtered out and not dispatched.
/// (The Fatal verbosity is an exception to this rule, see details below.)
///
/// This allows developers to control which messages are actually recorded or displayed without changing code.
///
/// Notes:
/// - Sinks (as defined by `ELogSinks`) determine the destinations of log messages (e.g., console, file, debugger).
/// - Verbosity levels are inclusive: a message with verbosity equal to the category's level is logged.
///
/// See also: ELogSinks, GMaxLogLevel.
enum class ELogVerbosity : uint8
{
    /// Always messages bypass normal verbosity filtering and are always dispatched to any enabled sinks.
    /// However, this verbosity level can be effectively suppressed by disabling all of a category's sinks.
    Always     = 0,

    /// For unrecoverable errors that prevent the program from safely continuing execution.
    /// Fatal messages bypass normal verbosity filtering and are dispatched to all sinks, even if disabled and all sinks
    /// are flushed to prepare for eventual termination of the program.
    /// Note that the logging system itself is not responsible for program termination.
    Fatal       = 1,

    /// For recoverable errors that should be addressed.
    Error       = 2,

    /// For warnings about potential issues or unexpected situations.
    Warning     = 3,

    /// For general informational messages about application state.
    Info        = 4,

    /// For additional detailed messages useful for debugging.
    Verbose     = 5,

    /// For very frequent, detailed messages which would otherwise spam the output log.
    VeryVerbose = 6,

    // Special sentinel value, not meant to be used directly
    Max = 255,
};

constexpr std::string_view ParseToString(ELogVerbosity Verbosity)
{
    switch (Verbosity)
    {
        case ELogVerbosity::Always:
            return "Always";
        case ELogVerbosity::Fatal:
            return "Fatal";
        case ELogVerbosity::Error:
            return "Error";
        case ELogVerbosity::Warning:
            return "Warning";
        case ELogVerbosity::Info:
            return "Info";
        case ELogVerbosity::Verbose:
            return "Verbose";
        case ELogVerbosity::VeryVerbose:
            return "VeryVerbose";
        case ELogVerbosity::Max:
            return "Max";
    }

    std::unreachable();
}

constexpr std::strong_ordering operator<=>(ELogVerbosity Lhs, ELogVerbosity Rhs)
{
    using T = std::underlying_type_t<ELogVerbosity>;
    return static_cast<T>(Lhs) <=> static_cast<T>(Rhs);
};
inline constinit ELogVerbosity GVerbosityMask = ELogVerbosity::Max;


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

    if (Sinks == ELogSinks::None || MessageVerbosity > CategoryVerbosity || MessageVerbosity > GVerbosityMask)
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

inline bool PassesVerbosityFilter(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask)
{
    const bool bSkipsFilter  = MessageVerbosity == ELogVerbosity::Fatal  || MessageVerbosity == ELogVerbosity::Always;
    const bool bPassesFilter = MessageVerbosity <= CategoryVerbosityMask && MessageVerbosity <= GVerbosityMask;
    return bSkipsFilter || bPassesFilter;
}

inline bool ShouldDispatch(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask, ELogSinks CategorySinks)
{
    if (MessageVerbosity == ELogVerbosity::Fatal) return true;
    if (CategorySinks    == ELogSinks::None)      return false;
    return PassesVerbosityFilter(MessageVerbosity, CategoryVerbosityMask);
}

inline void LogImplNew(std::string_view CategoryName, ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask,
             ELogSinks CategoryEnabledSinks, BasicFileSink& CategoryFileSink, std::string_view FormattedMessage_NoHeaders)
{
    if (!ShouldDispatch(MessageVerbosity, CategoryVerbosityMask, CategoryEnabledSinks))
    {
        return;
    }

    std::string FullyFormattedMessage;

    if (!SuppressLogheaders()) [[likely]]
    {
        std::string Header;
        if (!SupporesLogHeaderCategories())
        {
            Header += std::format("[{}] ", CategoryName);
        }
        if (!SuppresLogHeaderVerbosity())
        {
            Header += std::format("[{}] ", ParseToString(MessageVerbosity));
        }

        // One header for the beginning of the message plus one after every newline in the message itself
        const size_t NumHeaders = 1 + std::count(FormattedMessage_NoHeaders.begin(), FormattedMessage_NoHeaders.end(),
                                                 '\n');
        const size_t HeadersTotalSize = Header.size() * NumHeaders;
        const size_t FullyFormattedMessageSize = FormattedMessage_NoHeaders.size() + HeadersTotalSize;

        FullyFormattedMessage.reserve(FullyFormattedMessageSize);

        InsertLogHeaders(Header, FormattedMessage_NoHeaders, FullyFormattedMessage);
    }
    else
    {
        FullyFormattedMessage = FormattedMessage_NoHeaders;
    }

    DispatchToSinks(CategoryEnabledSinks, CategoryFileSink, FullyFormattedMessage);

    if (MessageVerbosity == ELogVerbosity::Fatal)
    {
        GStdErrSink().Flush();
        GStdOutSink().Flush();
        GLogFileSink().Flush();
        CategoryFileSink.Flush();
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

} // namespace Oggle
DECLARE_LOG_CATEGORY(Default, Info, Default)