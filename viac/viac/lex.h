#pragma once

#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cstdio>
#include<cctype> 
#include "util.h"

namespace viac
{
enum enum_TokenCode
{
	/* 运算符及分隔符 */
	TK_PLUS,							// + 加号
	TK_MINUS,							// - 减号
	TK_STAR,							// * 星号
	TK_DIVIDE,							// / 除号
	TK_MOD,								// % 求余运算符
	TK_EQ,								// == 等于号
	TK_NEQ,								// != 不等于号
	TK_LT,								// < 小于号
	TK_LEQ,								// <= 小于等于号
	TK_GT,								// > 大于号
	TK_GEQ,								// >= 大于等于号
	TK_ASSIGN,							// = 赋值运算符 
	TK_POINTSTO,						// -> 指向结构体成员运算符
	TK_DOT,								// . 结构体成员运算符
	TK_AND,								// & 地址与运算符
	TK_OPENPA,							// ( 左圆括号
	TK_CLOSEPA,							// ) 右圆括号
	TK_OPENBR,							// [ 左中括号
	TK_CLOSEBR,							// ] 右圆括号
	TK_BEGIN,							// { 左大括号
	TK_END,								// } 右大括号
	TK_SEMICOLON,						// ; 分号    
	TK_COMMA,							// , 逗号
	TK_ELLIPSIS,						// ... 省略号
	TK_EOF,								// 文件结束符
	TK_SPACE,							//
										/* 常量 */
	TK_CINT,							// 整型常量
	TK_CCHAR,							// 字符常量
	TK_CSTR,							// 字符串常量

	/* 关键字 */
	KW_CHAR,							// char关键字
	KW_SHORT,							// short关键字
	KW_INT,								// int关键字
	KW_VOID,							// void关键字  
	KW_STRUCT,							// struct关键字   
	KW_IF,								// if关键字
	KW_ELSE,							// else关键字
	KW_FOR,								// for关键字
	KW_CONTINUE,						// continue关键字
	KW_BREAK,							// break关键字   
	KW_RETURN,							// return关键字
	KW_SIZEOF,							// sizeof关键字
	KW_INCLUDE,
	KW_DO,
	KW_END,

	KW_REQUIRE,
	KW_ALIGN,							// __align关键字	
	KW_CDECL,							// __cdecl关键字 standard c call
	KW_STDCALL,							// __stdcall关键字 pascal c call

	/* 标识符 */
	TK_IDENT
};

enum enum_LexStatus
{
	LEX_NORMAL,
	LEX_SEP,
};

constexpr char File_EOF = -1;

constexpr int32_t MAXKEYSIZE = 4 * 1024;

class Symbol;

struct TkWord
{
	TkWord(const std::string& s, const int32_t& index) 
		: spelling(s)
		, tkcode(index)
		, symStruct(nullptr)
		, symId(nullptr)
	{
	}

	TkWord(const int32_t& index, const char* s)
		: spelling(s)
		, tkcode(index)
		, symStruct(nullptr)
		, symId(nullptr)
	{
	}

	TkWord(const std::string& s, const int32_t& index, std::shared_ptr<Symbol>& str, std::shared_ptr<Symbol>& id) 
		: spelling(s)
		, tkcode(index)
		, symStruct(str)
		, symId(id)
	{

	}
	int32_t tkcode;
	std::string spelling;
	std::shared_ptr<Symbol> symStruct;
	std::shared_ptr<Symbol> symId;
};

struct TkWordHash
{
	size_t operator()(const TkWord& w) const noexcept
	{
		return std::hash<std::string>{}(w.spelling);
	}
};

struct TkWordEqual
{
	bool operator()(const TkWord& l, const TkWord& r) const noexcept
	{
		return l.spelling == r.spelling;
	}
};

class Lex
{
public:
	std::unordered_map<std::string, int32_t> tkHashTable;
	std::vector<TkWord> tkTable;
	std::string sourceStr;
	std::string tkstr;
	char ch; 
	int32_t token;
	int32_t tkValue;
public:
	
	Lex(FILE* f):fin(f),ch(0), linenum(0)
	{

	}

	bool tkWordInsert(const std::string&);

	void tkWordDirectInsert(TkWord w);

	int tkWordFind(const std::string& str);

	inline void getCh()
	{
		if (fin)
		{
			ch = getc(fin);
		}
	}

	void preprocess();
	void parseIdentifier();
	bool isVaildCharacter(const char& ch);
	void parseNum();
	void parseString(const char& sep);
	void clearParseInfo();
	void initLex();
	void getToken();
	void skipWhiteSpace();
	void parseComment(const int32_t type);
	std::string  getTkstr(const int32_t index);
	void testLex();
	void colorToken(const int32_t lex_state);

	int64_t getFileLineNum() const noexcept
	{
		if (ch != File_EOF)
			return -1;
		return linenum;
	}
private:
	FILE * fin;
	int64_t linenum;
};

}
