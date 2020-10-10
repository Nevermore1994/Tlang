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

#define DEBUG 1

class Log
{
public:
    static Log& sharedInstance();
    
    void write();

    template<typename T>
    LogStream& addLog(const T& t) 
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            os_ << t;
        #if DEBUG
            std::cout << "write" << std::endl;
        #endif
            cond_.notify_one();
        }
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
    std::condition_variable cond_;
};


template<typename T>
LogStream& Logging(const T& t)
{
    return Log::sharedInstance().addLog(t);
}


}
