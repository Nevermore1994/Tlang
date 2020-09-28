#pragma once

#include<string>
#include<cstdio>
#include<iostream>
#include<mutex>

namespace T
{

using File_t = FILE*;

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
	
	enum class LogLevel
	{
		Waring = 0,
		Info,
		Error,
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

	//only linux
	std::string getColorText(const std::string& str, TextColor color, int32_t extraInfo = nil);
}

namespace FileUtil
{
	class File
	{
	public:
		explicit File(const std::string& path, int32_t flushInterval,  int32_t checkEveryN = 1024);

		explicit File(const char* path, int32_t flushInterval, int32_t checkEveryN = 1024);

		~File();
		
		File& operator=(const File&) = delete;
		
		File(const File&) = delete;
		
		void write(const std::string& str);
		
		void write(const char* str, uint32_t size);
		
		void flush();
	private:
		void init();
		
		void windUp();
	private:
		std::string path_;
		int32_t flushInterval_;
		int32_t checkEveryN_;
		File_t file_;
		uint64_t writeSize_;
		uint32_t writeCount_;
	};

}
};