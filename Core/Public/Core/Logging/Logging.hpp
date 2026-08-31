#pragma once
#include <../Utilities/Bitflags.hpp>
#include <../Vocabulary/OggleType.hpp>
#include <Core/Logging/Sinks.hpp>

/// Thread-safe logging system with configurable verbosity levels and output destinations.
///
/// # Overview
/// This logging system provides category-based message filtering and routing to multiple sinks.
/// Each log category independently controls its verbosity threshold and output destinations.
///
/// # Basic Usage
/// Declare a category (typically in a header)
/// DECLARE_LOG_CATEGORY(MyCategory, Info, Default)
///
/// Define it (in exactly one translation unit)
/// DEFINE_LOG_CATEGORY(MyCategory)
///
/// Log unformatted messages
/// LOG("Application started");                           // Uses Default category
/// LOG(MyCategory, "Cache miss");                        // Uses MyCategory with default verbosity/sinks
/// LOG(MyCategory, Warning, "Low memory");               // Specifies verbosity
/// LOG(MyCategory, Error, StdErr, "Critical failure");   // Specifies verbosity and sinks
///
/// // Log formatted messages
/// LOGFMT("Player count: {}", count);
/// LOGFMT(MyCategory, "Resource usage: {}/{}", current, max);
/// LOGFMT(Physics, Verbose, "Tick time: {:.2f}ms", deltaTime);
///
/// # Verbosity Filtering
/// Messages are filtered in two stages:
/// 1. Global maximum: GMaxLogLevel() applies across all categories
/// 2. Category threshold: Each category's Verbosity member
///
/// A message is processed only if its verbosity <= both thresholds.
/// Runtime verbosity can be adjusted per category: `LogMyCategory::Verbosity = ELogVerbosity::Error;`
///
/// # Sink Routing
/// After passing verbosity filters, messages route to configured sinks:
/// - StdOut: Console standard output
/// - StdErr: Console error output
/// - GlobalFile: Shared "Oggle_<timestamp>.log" in Logs/
/// - OwnFile: Category-specific "<CategoryName>_<timestamp>.log" in Logs/Log<CategoryName>/
///
/// Default sink configuration sends to StdOut | GlobalFile | OwnFile.
///
/// # Thread Safety
/// All sinks serialize concurrent writes. Messages from different threads may interleave
/// between calls but individual messages are atomic.
///
/// see ELogVerbosity for severity levels
/// see ELogSinks for output destination flags
/// see DECLARE_LOG_CATEGORY, DEFINE_LOG_CATEGORY for category definition
/// see LOG, LOGFMT for logging macros
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