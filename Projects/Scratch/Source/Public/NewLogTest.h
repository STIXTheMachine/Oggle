#pragma once
#include "Core/Logging/Logging.hpp"

namespace LogNewLogTest
{
    using namespace Oggle;
    inline constexpr std::string_view Name = "LogNewLogTest";
    inline constexpr ELogVerbosity DefaultMessageVerbosity = ELogVerbosity::Info;
    inline constexpr ELogVerbosity DefaultVerbosityMask = ELogVerbosity::Info;
    inline constexpr ELogSinks DefaultSinks = ELogSinks::Default;
    inline constinit ELogVerbosity VerbosityMask = DefaultVerbosityMask;
    inline constinit ELogSinks Sinks = DefaultSinks;
    inline void ResetMaxVerbosity() { VerbosityMask = DefaultVerbosityMask; } inline void ResetSinks() { Sinks = DefaultSinks; }
    inline BasicFileSink& GetCategoryFileSink() { static BasicFileSink Sink { "LogInName", "LogInName" }; return Sink; }
};