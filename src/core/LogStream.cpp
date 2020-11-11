#include<algorithm>
#include<limits>
#include<assert.h>
#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include<type_traits>
#include<ctime>

#include "LogStream.h"

using namespace T::Detail;

namespace T
{

namespace Detail
{
	const char digits[] = "9876543210123456789";
	const char* zero = digits + 9;
	static_assert(sizeof(digits) == 20, "wrong number of digits");

	const char digitsHex[] = "0123456789ABCDEF";
	static_assert(sizeof(digitsHex) == 17, "wrong number of digitsHex");

	template<typename T>
	size_t convert(char buff[], T value)
	{
		T i = value;
		char* p = buff;

		do
		{
			int lsd = static_cast<int> (i % 10);
			i /= 10;
			*p++ = zero[lsd];
		} while (i != 0);

		if (value < 0)
		{
			*p++ = '-';
		}
		*p = '\0';

		std::reverse(buff, p);

		return p - buff;
	}

	size_t convertHex(char buff[], uintptr_t value)
	{
		uintptr_t i = value;
		char* p = buff;

		do
		{
			int lsd = static_cast<int> (i % 16);
			i /= 16;
			*p++ = digitsHex[lsd];
		} while (i != 0);

		*p = '\0';
		std::reverse(buff, p);

		return p - buff;
	}

	template class FixedBuffer<iSmallBufferSize>;
	template class FixedBuffer<iMaxBufferSize>;
}

template<typename T>
inline void LogStream::formatInteger(T v)
{
	if (buffer_.avail() >= iMaxNumbericSize)
	{
		size_t len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}

LogStream& LogStream::operator<<(short v) noexcept
{
	*this << static_cast<int>(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned short v) noexcept
{
	*this << static_cast<unsigned int>(v);
	return *this;
}

LogStream& LogStream::operator<<(int v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long long v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) noexcept
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p) noexcept
{
	uintptr_t v = reinterpret_cast<uintptr_t> (p);
	if (buffer_.avail() >= iMaxNumbericSize)
	{
		char* buff = buffer_.current();
		buff[0] = '0';
		buff[1] = 'x';
		size_t len = Detail::convertHex(buff + 2, v);
		buffer_.add(len + 2);
	}

	return *this;
}

LogStream& LogStream::operator<<(double v) noexcept
{
	if (buffer_.avail() >= iMaxNumbericSize)
	{
		int len = snprintf(buffer_.current(), Detail::iMaxBufferSize, "%.12g", v);
		buffer_.add(len);
	}

	return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T val)
{
	static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");

	length_ = snprintf(buf_, sizeof(buf_), fmt, val);
	assert(static_cast<size_t> (length_) < sizeof(buf_));
}

template Fmt::Fmt(const char* fmt, char);
template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

LogStream& operator<<(LogStream& os, const tm& time) noexcept
{
	char str[128] = { 0 };
	strftime(str, sizeof(str), "%Y-%m-%d %H-%M-%S", &time);
	os << str;

	return os;
}

}