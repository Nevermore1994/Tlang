#pragma once

#include<string>
#include<cstdio>
#include <iostream>

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
	void output(const T& t)
	{
		output(std::cout, t);
	}

	template<typename T, typename ... Args>
	void output(const T& t, const Args& ... args)
	{
		output(std::cout, t, args...);
	}

	template<typename T>
	void outputLine(const T& t)
	{
		output(std::cout, t);
		std::cout << std::endl;
	}

	template<typename T, typename ... Args>
	void outputLine(const T& t, const Args& ... args)
	{
		output(std::cout, t, args...);
		std::cout << std::endl;
	}
	
	//only linux
	std::string getColorText(const std::string& str, TextColor color, int extraInfo = nil);
}

namespace FileUtil
{
	
}
};