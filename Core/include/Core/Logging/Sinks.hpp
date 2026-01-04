#pragma once
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace Oggle::Logging
{

using Message = std::string_view;

template<typename T>
concept LogSink = requires(T t, const Message& Msg, bool bForceFlush)
{
    { t.Write(Msg, bForceFlush) } -> std::same_as<void>;
    { t.WriteLn(Msg, bForceFlush) } -> std::same_as<void>;
    { t.Flush() } -> std::same_as<void>;
};

template<typename Derived>
struct ThreadSafeSinkBase
{
    void Write(const Message& Msg, bool bForceFlush = false)
    {
        std::lock_guard ScopeLock { Mutex };

        static_cast<Derived*>(this)->WriteImpl_AssumesLocked(Msg);

        if(bForceFlush)
        {
            static_cast<Derived*>(this)->FlushImpl_AssumesLocked();
        }
    };

    void WriteLn(const Message& Msg, bool bForceFlush = false)
    {
        std::lock_guard ScopeLock { Mutex };

        static_cast<Derived*>(this)->WriteImpl_AssumesLocked(Msg);
        static_cast<Derived*>(this)->WriteImpl_AssumesLocked("\n");

        if(bForceFlush)
        {
            static_cast<Derived*>(this)->FlushImpl_AssumesLocked();
        }
    };

    void Flush()
    {
        std::lock_guard ScopeLock { Mutex };
        static_cast<Derived*>(this)->FlushImpl_AssumesLocked();
    }

private:
    void WriteImpl_AssumesLocked(const Message& Msg) = delete;
    void FlushImpl_AssumesLocked() = delete;
    std::mutex Mutex;
};

struct BasicStdOutSink final : ThreadSafeSinkBase<BasicStdOutSink>
{
private:
    friend struct ThreadSafeSinkBase;
    void WriteImpl_AssumesLocked(const Message& Msg) { std::cout << Msg; };
    void FlushImpl_AssumesLocked() { std::cout << std::endl; }
};
static_assert(LogSink<BasicStdOutSink>);

struct BasicStdErrSink final : ThreadSafeSinkBase<BasicStdErrSink>
{
private:
    friend struct ThreadSafeSinkBase;
    void WriteImpl_AssumesLocked(const Message& Msg) { std::cerr << Msg; };
    void FlushImpl_AssumesLocked() { std::cerr << std::endl; }
};
static_assert(LogSink<BasicStdErrSink>);

struct BasicFileSink final : ThreadSafeSinkBase<BasicFileSink>
{
    explicit BasicFileSink(std::string_view BaseFilename)
    {
        using namespace std::chrono;
        using namespace std::filesystem;
        if (!exists(BaseLogDirectory))
        {
            create_directories(BaseLogDirectory);
        }

        auto Now = floor<seconds>(system_clock::now());
        const std::string TimestampedFilename = std::format("{}_{:%Y-%m-%d_%H-%M-%S}.log", BaseFilename, Now);

        const path Filepath = BaseLogDirectory / TimestampedFilename;
        File = std::ofstream { Filepath };
    }

    BasicFileSink(std::string_view BaseFilename, std::string_view Subfolder)
    {
        using namespace std::chrono;
        using namespace std::filesystem;
        path SubfolderPath { Subfolder };
        path LogFolderPath = BaseLogDirectory / SubfolderPath;

        if (!exists(LogFolderPath))
        {
            create_directories(LogFolderPath);
        }

        auto Now = floor<seconds>(system_clock::now());
        const std::string TimestampedFilename = std::format("{}_{:%Y-%m-%d_%H-%M-%S}.log", BaseFilename, Now);

        const path Filepath = LogFolderPath / TimestampedFilename;
        File = std::ofstream { Filepath };
    }

private:
    std::filesystem::path BaseLogDirectory { "Logs" };
    friend struct ThreadSafeSinkBase;
    void WriteImpl_AssumesLocked(const Message& Msg) { File << Msg; }
    void FlushImpl_AssumesLocked() { File.flush(); };
    std::ofstream File;
};
static_assert(LogSink<BasicFileSink>);
} // namespace Oggle::Logging