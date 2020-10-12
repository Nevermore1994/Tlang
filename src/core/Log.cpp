#include"Log.h"

using namespace T;
using namespace T::Util;
using namespace T::FileUtil;


namespace T
{

Log::Log()
    :file_("log", 10)
    ,work_("log thread", &Log::write, this)
    ,stop_(false)
{

}

Log::~Log()
{
    stop_ = true;
    file_.flush();
    cond_.notify_all();
    work_.stop();
}


void Log::write()
{
    std::string str;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [] { return !Log::sharedInstance().getLogStream().empty() || Log::sharedInstance().stop_;});
        if(stop_ && Log::sharedInstance().getLogStream().empty())
        {
            return;
        }
        str = os_.buffer().toString();
        os_.reset();
    }
    file_.write(str);
}

Log& Log::sharedInstance()
{
    static Log instance;
    return instance;
}

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
    " DEBUG ",
    " INFO  ",
    " WARN  ",
    " ERROR "
};

class S
{
public:
    S(const char* str, unsigned len):str_(str), len_(len)
    {
        assert(strlen(str) == len_);
    }

    const char* str_;
    const unsigned len_;
};


inline LogStream& operator<<(LogStream& s, S v)
{
    s.append(v.str_, v.len_);
    return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
    s.append(v.data_, v.size_);
    return s;
}


Logger::Impl::Impl(LogLevel level, const Logger::SourceFile &file, int line):
    stream_(Log::sharedInstance()),
    level_(level),
    line_(line),
    basename_(file)
{
    formatTime();
    stream_ << S(LogLevelName[level], strlen(LogLevelName[level]));
}

void Logger::Impl::formatTime()
{
    stream_ << getNowTime();
}

void Logger::Impl::finsh()
{
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line):impl_(INFO, file, line)
{

}

Logger::Logger(SourceFile file, int line, Enum_LogLevel level, const char* func):
    impl_(level, file, line)
{
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, Enum_LogLevel level):
    impl_(level, file, line)
{

}

Logger::~Logger()
{
    impl_.finsh();
}

} //end namespace T

