#include "Utility.h"
#include <cstdio>
#include <string>
#include <assert.h>
#include <time.h>
#include <thread>
#include <sys/time.h>

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
    char timebuf[128];
    struct timeval    tv;
    time_t now; 

    time(&now);
    gettimeofday(&tv, NULL); //msys2 env, this get localtime(&tv.tv_sec) is error, so in this way.
    struct tm* p = localtime(&now);
    snprintf(timebuf, 128, "%02d-%02d-%02d %02d:%02d:%02d.%06ld", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
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
    ,writeCount_(0)
    ,writeSize_(0)
{
    init();
}

File::File(const char* path, int32_t flushInterval, int32_t checkEveryN)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,file_(nullptr)
    ,writeCount_(0)
    ,writeSize_(0)
{
    init();
}

File::~File()
{
    windUp();
}

void File::init()
{
    Util::outputConsoleLine("init file");
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
    Util::outputConsoleLine(name, " begin write ....");
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
    //Util::outputConsoleLine("write file ", checkEveryN_, " ", writeCount_);
    if(writeCount_ >= checkEveryN_)
    {
        Util::outputConsoleLine("init file ", checkEveryN_, " ", writeCount_);
        windUp();
        init();
    }
}

   
}//end FileUtil

}


