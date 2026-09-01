#include "Core/Logging/Logging.hpp"
#include "Core/Vocabulary/Result.hpp"

DECLARE_LOG_CATEGORY(ResultTest, Info, StdOut | OwnFile)
DEFINE_LOG_CATEGORY(ResultTest)

void ResultTest()
{
    LOG(ResultTest, "Hello, ResultTest!")
}