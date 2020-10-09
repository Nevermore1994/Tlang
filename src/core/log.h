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

template<typename T>
void Logging(const T& t);

template<typename T, typename ... Args>
void Logging(const T& t, const Args & ...agrs);

class Log
{
public:
    static Log* sharedInstance();
    
    void write();

    template<typename T>
    LogStream& addLog(const T& t) 
    {
        std::unique_lock<std::mutex> lock(mutex_);
        os_ << t;
        cond_.notify_one();
        return os_;
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
    static Log* instance_;
    std::condition_variable cond_;
};

}
