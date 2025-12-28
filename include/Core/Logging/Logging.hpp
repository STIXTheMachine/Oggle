#pragma once
#include <Core/Bitflags.hpp>
#include <Core/OggleType.hpp>
#include <Core/Logging/Sinks.hpp>

namespace Oggle::Logging
{
/// Specifies the destinations (sinks) for log messages.
///
/// Each log message can be sent to one or more sinks, which control where the message is recorded or displayed.
/// These values can be combined using bitwise OR (`|`) because each sink represents a separate bit.
///
/// Example: ELogSinks sinks = ELogSinks::StdOut | ELogSinks::GlobalFile;
/// This will send the message to both the standard output console and the global log file.
///
/// Notes:
/// - Sinks are independent of verbosity; a message must pass the `ELogVerbosity` filter before being sent to the selected sinks.
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
/// - A default verbosity level (used if no custom level is set)
/// - A current verbosity level (used to filter messages at runtime)
///
/// When a log statement is made using `Log<MyCategory>(InVerbosity, InMessage)`:
/// 1. `InVerbosity` is compared to the current verbosity level of `MyCategory`.
/// 2. If `InVerbosity` > current verbosity, the message is **discarded**.
/// 3. If `InVerbosity` <= current verbosity, the message is **eligible to be sent** to the sinks configured for that category via `ELogSinks`.
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
    /// Logs critical errors and terminates the application.
    /// These messages are always processed regardless of category verbosity.
    Fatal       = 1,

    /// Logs recoverable errors that should be addressed.
    Error       = 2,

    /// Logs warnings about potential issues or unexpected situations.
    Warning     = 3,

    /// Logs general informational messages about application state.
    Info        = 4,

    /// Logs additional detailed messages useful for debugging.
    Verbose     = 5,

    /// Logs very detailed messages, often too verbose for normal use.
    VeryVerbose = 6,
};

constexpr std::strong_ordering operator<=>(ELogVerbosity Lhs, ELogVerbosity Rhs)
{
    using T = std::underlying_type_t<ELogVerbosity>;
    return static_cast<T>(Lhs) <=> static_cast<T>(Rhs);
};

/// Can be used to globally supress log messages across all categories. Use with caution.
/// Set to ELogVerbosity::VeryVerbose to disable global suppression.
/// See also: ELogVerbosity
ELogVerbosity& GMaxLogLevel();

#include <Core/Logging/LogImpl.inl>

} // namespace Oggle::Logging
DECLARE_LOG_CATEGORY(Default, Info, Default)