#pragma once

#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cstdio>

namespace viac
{
enum class enum_TokenCode
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

enum class enum_LexStatus
{
	LEX_NORML,
	LEX_SEP,
};

constexpr char File_EOF = -1;

constexpr int32_t MAXKEYSIZE = 4 * 1024;

class Symbol;

struct TkWord
{
	TkWord(const std::string& s, const int32_t& index) :spelling(s), tkcode(index), SymStruct(nullptr), Symid(nullptr)
	{

	}
	int32_t tkcode;
	std::string spelling;
	std::shared_ptr<Symbol> SymStruct;
	std::shared_ptr<Symbol> Symid;
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
	std::unordered_map<std::string, int> tkHashTable;
	std::vector<TkWord> tkTable;
	std::string SourceStr;
	std::string tkstr;
	char ch; 
	int32_t token;
	int32_t tkValue;
public:
	
	Lex(FILE* f):fin(f),ch(0)
	{

	}

	bool TkWordInsert(std::string&);

	void TkWordDirectInsert(TkWord w);

	int TkWordFind(const std::string& str);

	inline void GetCh()
	{
		if (fin)
		{
			ch = getc(fin);
		}
	}

	void Preprocess();
	void ParseIdentifier();
	void ParseNum();
	void ParseString(const char sep);
	void InitLex();
	void GetToken();
	int IsDigit(const char c);
	int IsNoDigit(const char c);
	void SkipWhiteSpace();
	void ParseComment(int type);
	std::string  GetTkstr(const int32_t index);
	void TestLex();
	void ColorToken(const int32_t lex_state);

	int64_t GetFileLineNum() const noexcept
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
