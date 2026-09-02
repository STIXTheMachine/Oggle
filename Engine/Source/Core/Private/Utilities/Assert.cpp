#include "Core/Utilities/Assert.hpp"
#include <sstream>
DEFINE_LOG_CATEGORY(Assert)

static std::string CleanCallstack(std::stacktrace& Stack)
{
    std::ostringstream CallStack;
    for (const auto& Entry : Stack)
    {
        if (to_string(Entry).contains("Oggle::Assert::Private::AssertInfo::AssertInfo()")) continue; // Strip out the AssertInfo constructor symbol
        CallStack << '\t' << Entry << '\n';
        if (to_string(Entry).starts_with("main at")) break;   // Stop the stacktrace when we get to main
    }
    return CallStack.str();
}

void Oggle::Assert::Private::AssertImpl(bool Condition, AssertInfo& InInfo)
{
    if (Condition) return;

    std::string ErrorMessage;

    if (InInfo.Message.empty())
    {
        ErrorMessage = std::format(
            "Assert failed at {}:{}:{}\nCallstack:\n{}",
            InInfo.Location.file_name(),
            InInfo.Location.line(),
            InInfo.Location.column(),
            CleanCallstack(InInfo.Stacktrace)
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Assert '{}' failed at {}:{}:{}\nCallstack:\n{}",
            InInfo.Message,
            InInfo.Location.file_name(),
            InInfo.Location.line(),
            InInfo.Location.column(),
            CleanCallstack(InInfo.Stacktrace)
        );
    }

    LOG(Assert, FMT("{}\nThis is a fatal error; program execution will be terminated.", ErrorMessage));
    std::abort();
}

void Oggle::Assert::Private::EnsureImpl(bool Condition, AssertInfo& InInfo)
{
    if (Condition) return;

    std::string ErrorMessage;

    if (InInfo.Message.empty())
    {
        ErrorMessage = std::format(
            "Ensure failed at {}:{}:{}\nCallstack:\n{}",
            InInfo.Location.file_name(),
            InInfo.Location.line(),
            InInfo.Location.column(),
            CleanCallstack(InInfo.Stacktrace)
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Ensure '{}' failed at {}:{}:{}\nCallstack:\n{}",
            InInfo.Message,
            InInfo.Location.file_name(),
            InInfo.Location.line(),
            InInfo.Location.column(),
            CleanCallstack(InInfo.Stacktrace)
        );
    }

    LOG(Assert, FMT("{}\nThis is NOT a fatal error; program execution will continue.", ErrorMessage));
}
