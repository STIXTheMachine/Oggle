#include "Core/Logging/Logging.hpp"

#if defined(OGGLE_ENABLE_LOGGING)
namespace Oggle::Private {
    bool SuppressLogHeaders()
    {
        return false;
    }

    bool SuppressLogHeaderCategories()
    {
        return false;
    }

    bool SuppressLogHeaderVerbosity()
    {
        return false;
    }

    BasicStdOutSink& GStdOutSink()
    {
        static BasicStdOutSink Sink;
        return Sink;
    };

    BasicStdErrSink& GStdErrSink()
    {
        static BasicStdErrSink Sink;
        return Sink;
    };

    BasicFileSink& GLogFileSink()
    {
        static BasicFileSink Sink { "Oggle" };
        return Sink;
    }

    void InsertLogHeaders(std::string_view Header, std::string_view Message, std::string& Output)
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

    void DispatchToSinks(ELogSinks Sinks, BasicFileSink& CategoryFileSink, std::string_view Message)
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

    bool PassesVerbosityFilter(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask)
    {
        const bool bSkipsFilter  = MessageVerbosity == ELogVerbosity::Fatal  || MessageVerbosity == ELogVerbosity::Always;
        const bool bPassesFilter = MessageVerbosity <= CategoryVerbosityMask && MessageVerbosity <= GVerbosityMask;
        return bSkipsFilter || bPassesFilter;
    }

    bool ShouldDispatch(ELogVerbosity MessageVerbosity, ELogVerbosity CategoryVerbosityMask, ELogSinks CategorySinks)
    {
        if (MessageVerbosity == ELogVerbosity::Fatal) return true;
        if (CategorySinks    == ELogSinks::None)      return false;
        return PassesVerbosityFilter(MessageVerbosity, CategoryVerbosityMask);
    }

    void LogImpl(
        std::string_view CategoryName,
        ELogVerbosity    CategoryVerbosityMask,
        ELogSinks        CategoryEnabledSinks,
        BasicFileSink&   CategoryFileSink,
        ELogVerbosity    MessageVerbosity,
        std::string_view FormattedMessage_NoHeaders
    )
    {
        if (!ShouldDispatch(MessageVerbosity, CategoryVerbosityMask, CategoryEnabledSinks))
        {
            return;
        }

        std::string FullyFormattedMessage;

        if (!SuppressLogHeaders()) [[likely]]
        {
            std::string Header;
            if (!SuppressLogHeaderCategories())
            {
                Header += std::format("[{}] ", CategoryName);
            }
            if (!SuppressLogHeaderVerbosity())
            {
                Header += std::format("[{}] ", ParseToString(MessageVerbosity));
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

        DispatchToSinks(CategoryEnabledSinks, CategoryFileSink, FullyFormattedMessage);

        if (MessageVerbosity == ELogVerbosity::Fatal)
        {
            GStdErrSink().Flush();
            GStdOutSink().Flush();
            GLogFileSink().Flush();
            CategoryFileSink.Flush();
        }
    };
} // namespace Oggle::Private
#endif