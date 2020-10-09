#include"log.h"

using namespace T;
using namespace T::Util;
using namespace T::FileUtil;


namespace T
{

template<typename T>
void Logging(const T& t)
{
    Log::sharedInstance()->addLog(t);
}

template<typename T, typename ... Args>
void Logging(const T& t, const Args & ...args)
{
    Log::sharedInstance()->addLog(t);
    Logging(args ...);
}


Log::Log()
    :file_("log", 10)
    ,work_("fem", &Log::write, this)
{
    instance_ = this;
}

Log::~Log()
{
    file_.flush();
    work_.stop();
}


void Log::write()
{
    std::string str;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return this->getLogStream().empty();});
        str = os_.buffer().toString();
        os_.reset();
    }
    file_.write(str);
}

Log* Log::sharedInstance()
{
    return instance_;
}

} //end namespace T

