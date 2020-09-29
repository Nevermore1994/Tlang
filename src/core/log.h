#pragma once

#include<mutex>
#include<condition_variable>
#include<atomic>
//#include<format>
#include"util.h"
using namespace std;
using namespace T;
using namespace T::Util;
using namespace T::FileUtil;

//todo format
void loger(const char* str, ...);

template<typename T>
void logex(const T& t)
{
    
}

template<typename T, typename ... Args>
void logex(const T& t, const Args& ... args)
{

}

class Log
{
public:
    static Log* sharedIntance();
    void func();
private:
    Log();
    ~Log();

private:
    Thread work_;
    File file_;
};
