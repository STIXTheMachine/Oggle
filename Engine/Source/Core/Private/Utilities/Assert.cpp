#include "Core/Utilities/Assert.hpp"
#include <sstream>

static std::string CleanCallstack(std::stacktrace& Stack)
{
    std::ostringstream CallStack;
    for (const auto& Entry : Stack)
    {
        if (to_string(Entry).contains("Oggle::Assert::Private::AssertInfo::AssertInfo()")) continue; // Strip out the AssertInfo constructor symbol
        CallStack << '\t' << Entry << '\n';
        if (to_string(Entry).contains("main at")) break;   // Stop the stacktrace when we get to main
    }
    return CallStack.str();
}

void Oggle::Private::Assert::AssertImpl(AssertInfo& Info)
{
    std::string ErrorMessage;

    if (Info.ErrorMessage.empty())
    {
        ErrorMessage = std::format(
            "Assert failed at {}:{}:{}\nCallstack:\n{}",
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Assert '{}' failed at {}:{}:{}\nCallstack:\n{}",
            Info.ErrorMessage,
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }

    LOG(Assert, FMT("{}\nThis is a fatal error; program execution will be terminated.", ErrorMessage));
    std::abort();
}

void Oggle::Private::Assert::EnsureImpl(AssertInfo& Info)
{
    std::string ErrorMessage;

    if (Info.ErrorMessage.empty())
    {
        ErrorMessage = std::format(
            "Ensure failed at {}:{}:{}\nCallstack:\n{}",
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Ensure '{}' failed at {}:{}:{}\nCallstack:\n{}",
            Info.ErrorMessage,
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }

    LOG(Ensure, FMT("{}\nThis is NOT a fatal error; program execution will continue.", ErrorMessage));
}
