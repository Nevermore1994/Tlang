#include "Utility.h"
#include <cstdio>
#include <string>
#include <assert.h>
#include <time.h>
#include <thread>
#include <cstring>
#if linux
#include <sys/time.h>
#else
#include <windows.h>
#endif // linux



namespace T
{

constexpr int kLogSize = 1024*1024*4;

namespace Util
{

void clearString(std::string & str)
{
    str.clear();
    str.resize(0);
}

#if linux
std::string getLogFileName(const std::string& basename)
{
	std::string filename;
	filename = basename;

	char timebuf[128];
	struct timeval tv;
	time_t now;

	time(&now);
	gettimeofday(&tv, NULL); //msys2 env, this get localtime(&tv.tv_sec) is error, so in this way.
	struct tm* p = localtime(&now);
	snprintf(timebuf, 128, "%02d-%02d-%02d#%02d-%02d-%02d-%06ld", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);

	filename.append(timebuf);
	filename.append(".log");

	return filename;
}

std::string getNowTime()
{
	char timebuf[128];
	struct timeval tv;
	time_t now;

	time(&now);
	gettimeofday(&tv, NULL); //msys2 env, this get localtime(&tv.tv_sec) is error, so in this way.
	struct tm* p = localtime(&now);
	snprintf(timebuf, 128, "%02d-%02d-%02d %02d:%02d:%02d.%06ld", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);
	return std::string(timebuf);
}

//https://www.cnblogs.com/crabxx/p/4046498.html

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
std::string getLogFileName(const std::string& basename)
{
	std::string filename = basename;
	filename.append(getNowTime());
	filename.append(".log");
	return filename;
}

std::string getNowTime()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char timebuf[128];
	snprintf(timebuf, 128, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	return std::string(timebuf);
}
#endif

int32_t calcAlign(int32_t n, int32_t align)
{
	int32_t num = n + align - 1;
	int32_t res = ~(align - 1);
	return (num & res);
}

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
    {
        outputConsoleLine(name_, " thread join..");
        worker_.join();
    }
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

#pragma region File

File::File(const std::string& path, FileMode mode)
    :path_(path)
    ,mode_(mode)
{
#if _WIN32
    fileName_ = path_.substr(path_.find_last_of("\\") + 1);
#else
    fileName_ = path_.substr(path_.find_last_of("/") + 1);
#endif
}

File::File(const char* path, FileMode mode)
    :path_(path)
    ,mode_(mode)
{

}

File::~File()
{
    if(file_)
    {
        fclose(file_);
        file_ = nullptr;
    }
}



#pragma endregion

#pragma region WriteFile

WriteFile::WriteFile(const std::string& path, bool isLogFile, uint32_t checkEveryN)
    :File(path, FileMode::WriteMode)
    ,isLogFile_(isLogFile)
    ,checkEveryN_(checkEveryN)
    ,writeCount_(0)
    ,writeSize_(0)
    ,checkCount_(0)
{
    init();
}

WriteFile::WriteFile(const char* path, bool isLogFile, uint32_t checkEveryN)
    :File(path, FileMode::WriteMode)
    ,isLogFile_(isLogFile)
    ,checkEveryN_(checkEveryN)
    ,writeCount_(0)
    ,writeSize_(0)
    ,checkCount_(0)
{
    init();
}

WriteFile::~WriteFile()
{
    windup();
}

void WriteFile::init()
{
    Util::outputConsoleLine("write init file");
    if(path_.empty())
        return;
    std::string name = path_;
    if(isLogFile_)
    {
        name = Util::getLogFileName(path_);
    }
    else
    {
        if(checkCount_)
            name.append(std::to_string(checkCount_));    
    }
    file_ = fopen(name.c_str(), Mode[(int)mode_]);
    if(!file_)
    {
        Util::outputConsoleLine("init file failed.", strerror(errno));
    }
    writeSize_ = 0;
    writeCount_ = 0;
    Util::outputConsoleLine(name, " begin write ....");
}

void WriteFile::windup()
{
    if(file_)
    {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

void WriteFile::flush()
{
    if(file_)
    {
        fflush(file_);
        writeCount_ = 0;
    }
}

void WriteFile::write(const std::string& str)
{
    write(str.c_str(), str.length());
}

void WriteFile::write(const char* str, uint32_t size)
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
        flush();
    }
    if(writeSize_ >= kLogSize)
    {
        windup();
        init();
    }
    
}
#pragma endregion 

#pragma region ReadFile

ReadFile::ReadFile(const std::string& path)
    :File(path, FileMode::ReadMode)
    ,readSize_(0)
    ,readOver_(false)
{
    init();
}

ReadFile::ReadFile(const char* path)
    :File(path, FileMode::ReadMode)
    ,readSize_(0)
    ,readOver_(false)
{
    init();
}

ReadFile::~ReadFile()
{
    windup();
}

char ReadFile::readCh()
{
    if(file_)
    {
        int ch = getc(file_);
        if(ch == EOF)
        {
            readOver_ = true;
        }
        readSize_ ++;
        return ch;
    }
    else
    {
        Util::outputConsoleLine("read ch error. file is invalid.");
    }

    return EOF;
}

void ReadFile::backfillCh(char ch)
{
    if(file_)
    {
        ungetc(ch, file_);
    }
    else
    {
        Util::outputConsoleLine("backfillCh error. file is invalid.");
    }
}

std::string ReadFile::readUntilCh(char ch)
{
   if(file_)
    {
        std::string res;
        int ch = getc(file_);
        while(ch != ch && ch != EOF)
        {
            res.append(1, ch);
            readSize_ ++;
            ch = getc(file_);
        }
        if(ch == EOF)
        {
            readOver_ = true;
        }
        return res;
    }
    else
    {
        Util::outputConsoleLine("read error. file is invalid.");
    }
    return "";
}

std::string ReadFile::readLine()
{
    return readUntilCh('\n');
}

std::string ReadFile::readWord()
{
    return readUntilCh(' ');
}

void ReadFile::init()
{
    Util::outputConsoleLine("init read file");
    if(path_.empty())
        return;
    file_ = fopen(path_.c_str(), Mode[(int)mode_]);
    if(!file_)
    {
        Util::outputConsoleLine("init read file failed.", strerror(errno));
        return;
    }
    readSize_ = 0;
    readOver_ = false;
    Util::outputConsoleLine(path_, " begin read ....");
}

bool ReadFile::switchFile(const char* path)
{
    path_ = path;
    init();
    return file_ != nullptr;
}

void ReadFile::windup()
{
    if(file_)
    {
        fclose(file_);
    }
}

#pragma endregion

#pragma region FreeFile

FreeFile::FreeFile(const std::string& path, FileMode mode)
    :File(path, FileMode::FreeMode)
    ,WriteFile(path, false)
    ,ReadFile(path)
{
    init();
}

FreeFile::FreeFile(const char* path, FileMode mode)
    :File(path, FileMode::FreeMode)
    ,WriteFile(path, false)
    ,ReadFile(path)
{
    init();
}

FreeFile::~FreeFile()
{
    windup();
}

void FreeFile::init() 
{
    Util::outputConsoleLine("init read file");
    if(path_.empty())
        return;
    file_ = fopen(path_.c_str(), Mode[(int)mode_]);
    if(!file_)
    {
        Util::outputConsoleLine("init read file failed.", strerror(errno));
    }
    Util::outputConsoleLine(path_, " begin read ....");
}

void FreeFile::windup()
{
    if(file_)
    {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

#pragma endregion

}//end FileUtil

}


