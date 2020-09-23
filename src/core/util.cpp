#include "util.h"
#include <cstdio>
#include <string>

namespace T
{

void Util::clearString(std::string & str)
{
    str.clear();
    str.resize(0);
}

//https://www.cnblogs.com/crabxx/p/4046498.html
std::string Util::getColorText(const std::string& str, TextColor color, int extraInfo)
{
    if(str.empty())
    {
        return str;
    }

    int c = static_cast<int>(color);
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


