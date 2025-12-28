namespace Oggle::Logging::Private
{
template<CLogCategory Category, typename... FormatArgs>
void LogImpl(ELogVerbosity MessageVerbosity, ELogSinks Sinks, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    #ifndef DISABLE_LOGGING
    // If fatal, log to all sinks and abort
    if (MessageVerbosity == ELogVerbosity::Fatal)
    {
        std::string Message = std::format(FormatString, std::forward<FormatArgs>(Args)...);

        GStdErr.WriteLn(Message, true);
        GStdOut.WriteLn(Message, true);
        GLogFile.WriteLn(Message, true);
        Category::SelfLogFile.WriteLn(Message, true);
        std::abort();
        return;
    }

    // Determine if log message should be emitted
    if (MessageVerbosity > Category::Verbosity || MessageVerbosity > GMaxLogLevel || Sinks == ELogSinks::None)
    {
        return;
    }

    std::string Message = std::format(FormatString, std::forward<FormatArgs>(Args)...);

    // Dispatch to sinks
    if (HasAnyFlags(Sinks, ELogSinks::StdErr))
    {
        GStdErr.WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::StdOut))
    {
        GStdOut.WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::GlobalFile))
    {
        GLogFile.WriteLn(Message);
    }
    if (HasAnyFlags(Sinks, ELogSinks::OwnFile))
    {
        Category::SelfLogFile.WriteLn(Message);
    }
    #endif
}
} // namespace Oggle::Logging::Private

// These functions are deliberately in the global namespace
template<Oggle::Logging::CLogCategory Category, typename... FormatArgs>
void Log(std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    Oggle::Logging::Private::LogImpl<Category>(Category::DefaultVerbosity, Category::DefaultSinks, FormatString, std::forward<FormatArgs>(Args)...);
};

template<Oggle::Logging::CLogCategory Category, typename... FormatArgs>
void Log(Oggle::Logging::ELogVerbosity OverrideVerbosity, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    Oggle::Logging::Private::LogImpl<Category>(OverrideVerbosity, Category::DefaultSinks, FormatString, std::forward<FormatArgs>(Args)...);
};

template<Oggle::Logging::CLogCategory Category, typename... FormatArgs>
void Log(Oggle::Logging::ELogSinks OverrideSinks, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    Oggle::Logging::Private::LogImpl<Category>(Category::DefaultVerbosity, OverrideSinks, FormatString, std::forward<FormatArgs>(Args)...);
};

template<Oggle::Logging::CLogCategory Category, typename... FormatArgs>
void Log(Oggle::Logging::ELogVerbosity OverrideVerbosity, Oggle::Logging::ELogSinks OverrideSinks, std::format_string<FormatArgs...> FormatString, FormatArgs&&... Args)
{
    Oggle::Logging::Private::LogImpl<Category>(OverrideVerbosity, OverrideSinks, FormatString, std::forward<FormatArgs>(Args)...);
};