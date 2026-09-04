#include "Core/Logging/Logging.hpp"

int main()
{
    Oggle::BasicFileSink TempSink { "Temp", "Temp" };
    Oggle::Private::LogImplNew("NotACategory", Oggle::ELogVerbosity::Info, Oggle::ELogVerbosity::Info, Oggle::ELogSinks::Default, TempSink, "Hello, World!");
}