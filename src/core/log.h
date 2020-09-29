#pragma once

#include<mutex>
#include<condition_variable>
#include<atomic>
#include"util.h"
using namespace std;
using namespace T;
using namespace T::Util;
using namespace T::FileUtil;

class Log
{
public:
    Log();
    ~Log();
};
