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

Util::Log::Log(const std::string& path,const int32_t& flushInterval, const int32_t& checkEveryN, bool isThreadSafe)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
    ,isThreadSafe_(isThreadSafe_)
{
    
}

Util::Log::Log(const char* path, const int32_t& flushInterval, const int32_t& checkEveryN, bool isThreadSafe)
    :path_(path)
    ,flushInterval_(flushInterval)
    ,checkEveryN_(checkEveryN)
{
    
}

//https://www.cnblogs.com/crabxx/p/4046498.html
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
    
    return std::move(s); 
}

}


