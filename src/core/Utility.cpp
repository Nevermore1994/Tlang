#include "Utility.h"
#include <cstdio>
#include <string>
#include <assert.h>
#include <time.h>
#include<thread>

namespace T
{

namespace Util
{

void clearString(std::string & str)
{
    str.clear();
    str.resize(0);
}

std::string getLogFileName(const std::string& basename)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    time_t now; 
    time(&now);
    
    struct tm* tm = localtime(&now);
    strftime(timebuf, sizeof(timebuf), "_%Y%m%d-%H%M%S", tm);
    filename += timebuf;
    filename += ".log";

    return filename;
}

std::string getNowTime()
{
    char timebuf[32];
    time_t now; 
    time(&now);
    
    struct tm* tm = localtime(&now);
    strftime(timebuf, sizeof(timebuf), "%Y/%m/%d-%H:%M:%S", tm);
    return std::string(timebuf);
}

//https://www.cnblogs.com/crabxx/p/4046498.html
#ifdef  linux
std::string getColorText(const std::string& str, TextColor color, int32_t extraInfo)
{
    if(str.empty())
    {
        return str;
    }

    int32_t c = static_cast<int32_t>(color);
    std::string s("\033[3");
    s.append(std::to_string(c));
    s.append(";");
    s.append(std::to_string(extraInfo));
    s.append("m");
    s.append(str);
    s.append("\033[0m");
    
    return s; 
}
#else
std::string getColorText(const std::string& str, TextColor color, int32_t extraInfo)
{
    return str;
}
#endif

Thread::Thread(const char* name)
    :name_(name)
    ,worker_(&Thread::func, this)
    ,isExit_(false)
    ,isRuning_(false)
{
    func_ = nullptr;
}


Thread::~Thread()
{
    if(worker_.joinable())
        worker_.join();
    outputConsoleLine(name_, " thread exit..");
}

void Thread::stop()
{
    isExit_ = true;
    if(worker_.joinable())
        worker_.join();
    outputConsoleLine(name_, " thread stop..");
}

void Thread::wait()
{
    isRuning_ = false;
}

void Thread::wakeUp()
{
    isRuning_ = true;
}

void Thread::func()
{
    while(!isExit_)
    {
        if(isRuning_ && func_)
        {
            func_();
        }
    }
}

} // end Util

namespace FileUtil
{

File::File(const std::string& path, int32_t flushInterval, int32_t checkEveryN)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,file_(nullptr)
{
    init();
}

File::File(const char* path, int32_t flushInterval, int32_t checkEveryN)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,file_(nullptr)
{
    init();
}

File::~File()
{
    windUp();
}

void File::init()
{
    if(path_.empty())
        return;
    std::string name = Util::getLogFileName(path_);
    {
        file_ = fopen(name.c_str(), "wb");
    }
    if(!file_)
    {
        Util::outputConsoleLine("open file failed.");
    }
    writeSize_ = 0;
    writeCount_ = 0;
    Util::outputConsoleLine("begin write ....");
}

void File::windUp()
{
    if(file_)
    {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

void File::flush()
{
    if(file_)
    {
        fflush(file_);
    }
}

void File::write(const std::string& str)
{
    write(str.c_str(), str.length());
}

void File::write(const char* str, uint32_t size)
{
    if(!file_)
    {
        Util::outputConsoleLine("write file failed.");
        return;
    }
    writeCount_ ++;
    size = fwrite(str, 1 , size, file_);
    writeSize_ += size;
    if(writeCount_ >= checkEveryN_)
    {
        windUp();
        init();
    }
}

   
}//end FileUtil

}


