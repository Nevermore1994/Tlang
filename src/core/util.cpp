#include "util.h"
#include <cstdio>
#include <string>
#include <assert.h>
#include <time.h>

namespace T
{

void Util::clearString(std::string & str)
{
    str.clear();
    str.resize(0);
}

std::string Util::getLogFileName(const std::string& basename)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    time_t now = time(nullptr);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;
    filename += ".log";

    return filename;
}

std::string Util::getNowTime()
{
    tm tm_time;
    char t_time[32];
    int len  = snprintf(t_time, sizeof (t_time), "%4d%02%02d %02d:%02d:%02d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    return std::string(t_time);
}

//https://www.cnblogs.com/crabxx/p/4046498.html
#ifdef  __GNUC__
std::string Util::getColorText(const std::string& str, TextColor color, int32_t extraInfo)
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
std::string Util::getColorText(const std::string& str, TextColor color, int32_t extraInfo)
{
    return str;
}
#endif



FileUtil::File::File(const std::string& path, int32_t flushInterval, int32_t checkEveryN)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,file_(nullptr)
{
    init();
}

FileUtil::File::File(const char* path, int32_t flushInterval, int32_t checkEveryN)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,file_(nullptr)
{
    init();
}

FileUtil::File::~File()
{
    windUp();
}

void FileUtil::File::init()
{
    windUp();
    if(path_.empty())
        return;
    file_ = fopen(Util::getLogFileName(path_).c_str(), "wb");
    if(!file_)
    {
        Util::outputConsoleLine("open file failed.");
    }
    writeSize_ = 0;
    writeCount_ = 0;
    Util::outputConsoleLine("begin write ....");
}

void FileUtil::File::windUp()
{
    if(file_)
    {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

void FileUtil::File::write(const std::string& str)
{
    write(str.c_str(), str.length());
}

void FileUtil::File::write(const char* str, uint32_t size)
{
    if(!file_)
    {
        Util::outputConsoleLine("write log failed.");
    }
    writeCount_ ++;
    size = fwrite(str, 1 , size, file_);
    writeSize_ += size;
    if(writeCount_ >= checkEveryN_)
    {
        init();
    }
}


}


