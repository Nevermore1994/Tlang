#include"log.h"
using namespace T;
using namespace T::Util;
using namespace T::FileUtil;
Log::Log()
    :file_("log", 10)
    ,work_("fem", &Log::func)
{

}

Log::~Log()
{
    file_.flush();
}