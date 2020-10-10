#pragma once

#include<cassert>
#include<cstring>
#include<string>

namespace T
{
namespace Detail
{
const int iSmallBufferSize = 4000;
const int iMaxBufferSize = 4000 * 1000;

template<size_t SIZE>
class FixedBuffer
{
public:
	FixedBuffer() :cur_(data_) {}
	~FixedBuffer() = default;

	const char* data() const noexcept { return data_; }
	int length()const noexcept { return static_cast<int>(cur_ - data_); }
	char* current() noexcept { return cur_; }
	int avail() const noexcept { return static_cast<int> (end() - cur_); }
	void add(size_t len) noexcept { cur_ += len; }
	void append(const char* ptr, size_t len) noexcept
	{
		if (static_cast<size_t> (avail()) > len)
		{
			memcpy(cur_, ptr, len);
			cur_ += len;
		}
	}

	void reset() noexcept { cur_ = data_; }
	
	void bzero() noexcept { memset(data_, 0, sizeof(data_)); reset(); }

    std::string toString() const { return std::string(data_, length()); }
	
	FixedBuffer(const FixedBuffer&) = delete;
	
	FixedBuffer& operator=(const FixedBuffer&) = delete;
private:
	char* cur_;
	char data_[SIZE] = { 0 };

	const char* end() const noexcept { return (data_ + sizeof(data_)); }
};
}

class LogStream 
{
	using self = LogStream;

	using Buffer = Detail::FixedBuffer<Detail::iSmallBufferSize>;
public:
	LogStream() = default;
	
	~LogStream() = default;
	
	LogStream(const LogStream& ) = delete;

	LogStream& operator=(const LogStream&) = delete;
	
	self& operator<< (bool v) noexcept
	{
		buffer_.append(v ? "1" : "0", 1);
		return *this;
	}

	self& operator<<(short) noexcept;
	self& operator<<(unsigned short) noexcept;
	self& operator<<(int) noexcept;
	self& operator<<(unsigned int) noexcept;
	self& operator<<(long) noexcept;
	self& operator<<(unsigned long) noexcept;
	self& operator<<(long long) noexcept;
	self& operator<<(unsigned long long) noexcept;

	self& operator<<(const void*) noexcept;
	self& operator<<(double v) noexcept;
	self& operator<<(float v) noexcept
	{
		*this << static_cast<double>(v);
		return *this;
	}

	self& operator<<(char v) noexcept
	{
		buffer_.append(&v, 1);
		return *this;
	}

	self& operator<<(const char* pstr) noexcept
	{
		pstr ? buffer_.append(pstr, strlen(pstr)) : buffer_.append("(null)", 6);
		return *this;
	}

	self& operator<<(const unsigned char* pstr) noexcept
	{
		return operator<<(reinterpret_cast<const char*>(pstr));
	}

	self& operator<<(const std::string& str) noexcept
	{
		buffer_.append(str.c_str(), str.size());
		return *this;
	}

	self& operator<<(Buffer& buff) noexcept
	{
		*this << buff.toString();
		return *this;
	}

	void append(const char* pstr, const int len) noexcept { buffer_.append(pstr, len); }
	const Buffer& buffer() noexcept { return buffer_; }
	void reset() noexcept { buffer_.reset(); }
	void clear() noexcept { buffer_.bzero(); }
	int length() noexcept { return buffer_.length(); }    //DeBug
	bool empty() noexcept { return buffer_.length() > 0;}

	const char* print() const { return buffer_.data(); }
private:
	Buffer buffer_;

	template<typename T>
	void formatInteger(T v);

	static const int iMaxNumbericSize = 32;
};

class Fmt
{
public:
	template<typename T>
	Fmt(const char* fmt, T val);
	~Fmt() = default;

    const char* data() const { return buf_; }
	int length() const { return length_; }
private:
	char buf_[32] = { 0 };
	int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
    s.append(fmt.data(), fmt.length());
    return s;
}

LogStream& operator<<(LogStream& os, const tm& time) noexcept;

} //namespace T
