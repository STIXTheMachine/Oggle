#include "Core/Utilities/Assert.hpp"
#include <sstream>


static std::string GetCheckedExpression(std::source_location& Location)
{
    // Find the opening parenthesis of OGGLE_ASSERT() or OGGLE_ENSURE() (or their _MSG counterparts)

    std::string SourceFile;

    {
        std::ifstream SourceFileStream { Location.file_name() };
        SourceFile = std::string { std::istreambuf_iterator(SourceFileStream), {} };
    }

    size_t ErrorLineNumber = Location.line();

    size_t ErrorLineStartPos = 0;

    while (--ErrorLineNumber)
    {
        ErrorLineStartPos = SourceFile.find('\n', ErrorLineStartPos + 1);
    }

    size_t ErrorLineEndPos = SourceFile.find('\n', ErrorLineStartPos + 1);

    size_t MacroBeginPos = SourceFile.find("OGGLE_", ErrorLineStartPos);
    size_t MacroOpenParenPos = SourceFile.find('(', MacroBeginPos + 1);
    size_t MacroCloseParenPos = MacroOpenParenPos + 1;

    size_t NumOpenParens = 1;
    for (auto It = SourceFile.begin() + MacroOpenParenPos + 1; It != SourceFile.end(); ++It)
    {
        MacroCloseParenPos++;
        if (*It == '(') ++NumOpenParens;
        else if (*It == ')') --NumOpenParens;

        if (NumOpenParens == 0) break;
    }

    size_t ConditionLength = MacroCloseParenPos - MacroOpenParenPos;

    std::string_view MacroArgs  { SourceFile.begin() + MacroOpenParenPos + 1, SourceFile.begin() + MacroCloseParenPos - 1 };
    std::string_view Expression = MacroArgs;

    if (MacroArgs.contains(','))
    {
        size_t CommaPos = Expression.find(',');
        Expression = { MacroArgs.begin(), MacroArgs.begin() + CommaPos };
    }

    return std::string { Expression };
}

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

void Oggle::Private::Assert::AssertImpl(ErrorInfo& Info)
{
    std::string ErrorMessage;

    if (Info.ErrorMessage.empty())
    {
        ErrorMessage = std::format(
            "Assertion '{}' failed at {}:{}:{}\nCallstack:\n{}",
            GetCheckedExpression(Info.Location),
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Assertion '{}' failed at {}:{}:{}\nError Message: {}\nCallstack:\n{}",
            GetCheckedExpression(Info.Location),
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.ErrorMessage,
            Info.Stacktrace
        );
    }

    LOG(Assert, FMT("{}\nThis is a fatal error; program execution will be terminated.", ErrorMessage));
    std::abort();
}

void Oggle::Private::Assert::UnimplementedImpl(ErrorInfo& Info, const char* FunctionName)
{
    std::string ErrorMessage = std::format(
        "Call to unimplemented function {} at {}:{}:{}\nCallstack:\n{}",
        FunctionName,
        Info.Location.file_name(),
        Info.Location.line(),
        Info.Location.column(),
        Info.Stacktrace
    );

    LOG(Assert, FMT("{}\nThis is a fatal error; program execution will be terminated.", ErrorMessage));
    std::abort();
}

void Oggle::Private::Assert::EnsureImpl(ErrorInfo& Info)
{
    std::string ErrorMessage;

    if (Info.ErrorMessage.empty())
    {
        ErrorMessage = std::format(
            "Ensure expression '{}' failed at {}:{}:{}\nCallstack:\n{}",
            GetCheckedExpression(Info.Location),
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.Stacktrace
        );
    }
    else
    {
        ErrorMessage = std::format(
            "Ensure expression '{}' failed at {}:{}:{}\nError Message: {}\nCallstack:\n{}",
            GetCheckedExpression(Info.Location),
            Info.Location.file_name(),
            Info.Location.line(),
            Info.Location.column(),
            Info.ErrorMessage,
            Info.Stacktrace
        );
    }

    LOG(Ensure, FMT("{}\nThis is NOT a fatal error; program execution will continue.", ErrorMessage));
}
