#pragma once

#if defined(OGGLE_ENABLE_LOGGING)
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

// The global verbosity mask. This can be used to suppress
inline constinit auto GVerbosityMask = ELogVerbosity::Max;

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

#define LOG_CATEGORY_STRINGIFY_IMPL(X) #X
#define LOG_CATEGORY_STRINGIFY(X) LOG_CATEGORY_STRINGIFY_IMPL(X)

#define LOG_CATEGORY_NAME_PASTE(Name) Log##Name
#define LOG_CATEGORY_NAME(Name) LOG_CATEGORY_NAME_PASTE(Name)
#define LOG_CATEGORY_NAME_STRING(Name) LOG_CATEGORY_STRINGIFY(LOG_CATEGORY_NAME(Name))

// Make sure this macro is not invoked inside any namespaces!
#define DECLARE_LOG_CATEGORY(CategoryName, CategoryDefaultMessageVerbosity, CategoryDefaultVerbosityMask, CategoryDefaultSinks) \
namespace Oggle::LOG_CATEGORY_NAME(CategoryName) \
{ \
    static constexpr std::string_view      Name                    = LOG_CATEGORY_NAME_STRING(CategoryName); \
    static constexpr Oggle::ELogVerbosity  DefaultMessageVerbosity = Oggle::ELogVerbosity::CategoryDefaultMessageVerbosity; \
    static constexpr Oggle::ELogVerbosity  DefaultVerbosityMask    = Oggle::ELogVerbosity::CategoryDefaultVerbosityMask; \
    static constexpr Oggle::ELogSinks      DefaultSinks            = Oggle::ELogSinks::CategoryDefaultSinks; \
    \
    static constinit Oggle::ELogVerbosity  VerbosityMask           = DefaultVerbosityMask; \
    static constinit Oggle::ELogSinks      EnabledSinks            = DefaultSinks; \
    \
    static           void                  ResetVerbosityMask() { VerbosityMask = DefaultVerbosityMask; } \
    static           void                  ResetSinks()         { EnabledSinks = DefaultSinks; } \
    static           Oggle::BasicFileSink& GetFileSink()        { static Oggle::BasicFileSink Sink { LOG_CATEGORY_NAME_STRING(InName), LOG_CATEGORY_NAME_STRING(InName) }; return Sink; } \
};

#define FMT(...) std::format(__VA_ARGS__)

#define LOG_1(LogString) Oggle::Private::LogImpl(Oggle::LogDefault::Name, Oggle::LogDefault::VerbosityMask, Oggle::LogDefault::EnabledSinks, Oggle::LogDefault::GetFileSink(), Oggle::LogDefault::DefaultMessageVerbosity, LogString)

#define LOG_2(Category, Message) Oggle::Private::LogImpl(Oggle::LOG_CATEGORY_NAME(Category)::Name, Oggle::LOG_CATEGORY_NAME(Category)::VerbosityMask, Oggle::LOG_CATEGORY_NAME(Category)::EnabledSinks, Oggle::LOG_CATEGORY_NAME(Category)::GetFileSink(), Oggle::LOG_CATEGORY_NAME(Category)::DefaultMessageVerbosity, Message)

#define LOG_3(Category, MessageVerbosity, Message) Oggle::Private::LogImpl(Oggle::LOG_CATEGORY_NAME(Category)::Name, Oggle::LOG_CATEGORY_NAME(Category)::VerbosityMask, Oggle::LOG_CATEGORY_NAME(Category)::EnabledSinks, Oggle::LOG_CATEGORY_NAME(Category)::GetFileSink(), Oggle::ELogVerbosity::MessageVerbosity, Message)

#define LOG_4(Category, MessageVerbosity, OverrideSinks, Message) Oggle::Private::LogImpl(Oggle::LOG_CATEGORY_NAME(Category)::Name, Oggle::LOG_CATEGORY_NAME(Category)::VerbosityMask, Oggle::LOG_CATEGORY_NAME(Category)::EnabledSinks, Oggle::ELogSinks::OverrideSinks, Oggle::ELogVerbosity::MessageVerbosity, Message)

#define DISPATCH_LOG(_1, _2, _3, _4, NUM_ARGS, ...) NUM_ARGS

#define LOG(...) DISPATCH_LOG(__VA_ARGS__, LOG_4, LOG_3, LOG_2, LOG_1)(__VA_ARGS__)

namespace Private // Oggle::Private
{
bool SuppressLogHeaders();
bool SuppressLogHeaderCategories();
bool SuppressLogHeaderVerbosity();

BasicStdOutSink& GStdOutSink();
BasicStdErrSink& GStdErrSink();
BasicFileSink&   GLogFileSink();

bool PassesVerbosityFilter(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask);

bool ShouldDispatch(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask, ELogSinks CategorySinks);

void InsertLogHeaders(std::string_view Header, std::string_view Message, std::string& Output);

void DispatchToSinks(ELogSinks Sinks, BasicFileSink& CategoryFileSink, std::string_view Message);

void LogImpl(
    std::string_view CategoryName,
    ELogVerbosity    CategoryVerbosityMask,
    ELogSinks        CategoryEnabledSinks,
    BasicFileSink&   CategoryFileSink,
    ELogVerbosity    MessageVerbosity,
    std::string_view FormattedMessage_NoHeaders
);

} // namespace Private
} // namespace Oggle

DECLARE_LOG_CATEGORY(Default, Info, Info, Default)

#else
#define LOG(...)
#define FMT(...)
#define DECLARE_LOG_CATEGORY(...)
#endif