#pragma once

#include<mutex>
#include<condition_variable>
#include<atomic>
//#include<format>
#include"Utility.h"
#include"LogStream.h"

using namespace T;
using namespace T::Util;
using namespace T::FileUtil;

namespace T
{

class Log
{
public:
    static Log& sharedInstance();
    
    void write();

    template<typename T>
    Log& operator<<(const T& t) 
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            os_  << t;
            cond_.notify_one();
        }

        return sharedInstance();
    }

    LogStream& getLogStream() { return os_; } 
private:
    Log();
    ~Log();
private:
    Thread work_;
    File file_;
    std::mutex mutex_;
    LogStream os_;
    std::condition_variable cond_;
};


class Logger
{
public:
    enum Enum_LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        NUM_LOG_LEVELS,
    };

    class SourceFile
    {
    public:
        template<int N>
        SourceFile(const char(&arr)[N]):data_(arr),size_(N-1)
        {
            const char* slash = strrchr(data_, '/');
            if(slash)
            {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char* filename) :data_(filename)
        {
            const char* slash = strrchr(filename, '/');
            if(slash)
            {
                data_ = slash + 1;
            }
            size_ = static_cast<int>(strlen(data_));
        }
        const char* data_;
        int size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, Enum_LogLevel level);
    Logger(SourceFile file, int line, Enum_LogLevel level, const char* func);
    ~Logger();

    LogStream& stream()
    {
        return impl_.stream_.getLogStream();
    }

    static Enum_LogLevel logLevel();
    static void setLogLevel(Enum_LogLevel level);
private:
    class Impl
    {
    public:
        using LogLevel = Logger::Enum_LogLevel ;
        Impl(LogLevel level, const SourceFile& file, int line);

        void formatTime();

        void finsh();

        Log& stream_;
        LogLevel level_;
        int line_;
        SourceFile basename_;
    };

    Impl impl_;
};

#define LOG_DEBUG T::Logger(__FILE__,  __LINE__, T::Logger::DEBUG, __FUNCTION__).stream()

#define LOG_WARN T::Logger(__FILE__, __LINE__, T::Logger::WARN).stream()

#define LOG_ERROR T::Logger(__FILE__, __LINE__, T::Logger::ERROR).stream()

#define LOG_INFO T::Logger(__FILE__,  __LINE__, T::Logger::INFO).stream()

}
