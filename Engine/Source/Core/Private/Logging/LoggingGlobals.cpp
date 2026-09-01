#include "Core/Logging/Logging.hpp"

namespace Oggle::Logging
{
namespace Private {
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
    };
} // namespace Private

ELogVerbosity& GMaxLogLevel()
{
    static ELogVerbosity GlobalMax { ELogVerbosity::VeryVerbose };
    return GlobalMax;
}
} // namespace Oggle::Logging

DEFINE_LOG_CATEGORY(Default)