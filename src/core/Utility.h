#pragma once

#include<string>
#include<cstdio>
#include<iostream>
#include<mutex>
#include<thread>
#include<functional>
#include<atomic>

namespace T
{
constexpr int nil = -1;

namespace Util
{
enum class TextColor
{
	Black = 0,
	Green,
	Red,
	Yellow,
	Blue,
	Purple,
	Cyan,
	White
};

//cross platform
void clearString(std::string& str);

template<typename T>
std::ostream& output(std::ostream& os, const T& t)
{
	return os << t;
}

template<typename T, typename ... Args>
std::ostream& output(std::ostream& os, const T& t, const Args& ... args)
{
	os << t;
	return output(os, args...);
}

template<typename T>
void outputConsole(const T& t)
{
	output(std::cout, t);
}

template<typename T, typename ... Args>
void outputConsole(const T& t, const Args& ... args)
{
	output(std::cout, t, args...);
}

template<typename T>
void outputConsoleLine(const T& t)
{
	output(std::cout, t);
	std::cout << std::endl;
}

template<typename T, typename ... Args>
void outputConsoleLine(const T& t, const Args& ... args)
{
	output(std::cout, t, args...);
	std::cout << std::endl;
}

std::string getLogFileName(const std::string& basename);

std::string getNowTime();

int32_t calcAlign(int32_t n, int32_t align);


//only linux
std::string getColorText(const std::string& str, TextColor color, int32_t extraInfo = nil);


class Thread
{
public:
	template<class Func, typename ... Args>
	Thread(const char* name,Func&& f, Args&& ... args)
		:name_(name)
		,worker_(&Thread::func, this)
		,isExit_(false)
		,isRuning_(true)
	{
		func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
	}
	Thread(const char* name);
	~Thread();
	inline const std::string& geThreadName() const { return name_; };
	void wait();
	void wakeUp();
	void stop();
	template<class Func, typename ... Args>
	void setFunc(Func&& f, Args&& ... args)
	{
		func_ = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
		isRuning_ = true;
	}

	inline bool isRuning() const { return isRuning_.load(); };
private:
	void func();
private:
	std::thread worker_;
	std::string name_;
	std::function<void()> func_;
	std::atomic_bool isRuning_;
	std::atomic_bool isExit_;
};

} //end Util

namespace FileUtil
{
using File_t = FILE*;


constexpr const char* Mode[] = {"rb", "wb", "wb+"};
enum class FileMode
{
	ReadMode = 0, 
	WriteMode = 1,
	FreeMode = 2
};

class File
{
public:
	explicit File(const std::string& path, FileMode mode);
	explicit File(const char* path, FileMode mode);
	File& operator=(const File&) = delete;
	File(const File&) = delete;
	virtual ~File();
private:
	virtual void init() = 0;
	virtual void windup() = 0;
public:
	std::string path_;
	File_t file_;
	FileMode mode_;
};

class WriteFile:virtual public File
{
public:
	explicit WriteFile(const std::string& path,  bool isLogFile, uint32_t checkEveryN = 4096);
	explicit WriteFile(const char* path, bool isLogFile, uint32_t checkEveryN = 4096);
	virtual ~WriteFile();
	void write(const std::string& str);
	void write(const char* str, uint32_t size);
	void flush();
private:
	void init() override;
	void windup() override;
private:
	uint32_t checkEveryN_;
	uint64_t writeSize_;
	uint32_t writeCount_;
	bool isLogFile_;
	uint32_t checkCount_;
};

class ReadFile:virtual public File
{
public:
	explicit ReadFile(const char* path);
	explicit ReadFile(const std::string& path);
	virtual ~ReadFile();
	std::string readLine();
	char readCh();
	void backfillCh(char ch);
	std::string readWord();
	inline bool readOver() const { return readOver_;}
	inline uint64_t readSize() const { return readSize_; }
private:
	void init() override;
	void windup() override;
	std::string readUntilCh(char ch);
private:
	bool readOver_;
	uint64_t readSize_;
};

class FreeFile final:public ReadFile, public WriteFile
{
public:
	FreeFile(const std::string& path, FileMode mode);
	FreeFile(const char* path, FileMode mode);
	virtual ~FreeFile();
private:
	void init() final;
	void windup() final;
};

}//end namespace FileUtil

} //end namespace T