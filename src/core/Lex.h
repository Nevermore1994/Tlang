#pragma once

#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cctype> 
#include"Utility.h"

namespace T
{
enum enum_TokenCode
{
	TK_PLUS,							// +
	TK_MINUS,							// -
	TK_STAR,							// *
	TK_DIVIDE,							// /
	TK_MOD,								// % 
	TK_EQ,								// ==
	TK_NEQ,								// !=
	TK_LT,								// <
	TK_LEQ,								// <=
	TK_GT,								// > 
	TK_GEQ,								// >=
	TK_ASSIGN,							// = 
	TK_POINTSTO,						// ->
	TK_DOT,								// . 
	TK_AND,								// & 
	TK_OPENPA,							// ( 
	TK_CLOSEPA,							// ) 
	TK_OPENBR,							// [ 
	TK_CLOSEBR,							// ] 
	TK_BEGIN,							// { 
	TK_END,								// } 
	TK_SEMICOLON,						// ; 
	TK_COMMA,							// , 
	TK_ELLIPSIS,						// ...
	TK_EOF,								// 
	TK_SPACE,							//
										
	TK_CINT,							// 
	TK_CCHAR,							// 
	TK_CSTR,							// 

	KW_CHAR,							// char
	KW_SHORT,							// short
	KW_INT,								// int
	KW_VOID,							// void
	KW_STRUCT,							// struct
	KW_IF,								// if
	KW_ELSE,							// else
	KW_FOR,								// for
	KW_CONTINUE,						// continue
	KW_BREAK,							// break
	KW_RETURN,							// return
	KW_SIZEOF,							// sizeof
	KW_INCLUDE,
	KW_DO,
	KW_END,

	KW_REQUIRE,
	KW_ALIGN,							// __align
	KW_CDECL,							// __cdecl  call
	KW_STDCALL,							// __stdcall

	TK_IDENT
};

enum enum_LexStatus
{
	LEX_NORMAL,
	LEX_SEP,
};

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
	
	Lex(const std::string& path);

	Lex(const char* path);

	bool tkWordInsert(const std::string&);

	void tkWordDirectInsert(TkWord w);

	int tkWordFind(const std::string& str);

	inline void getCh()
	{
		ch_ = file_.readCh();
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
	std::string getTkstr(int32_t index);
	void testLex();
	void colorToken(const int32_t lex_state);

	int64_t getFileLineNum() const noexcept
	{
		if (ch_ != -1)
			return -1;
		return linenum_;
	}
public:
	static std::unordered_map<std::string, int32_t> TokenHashTable;
	static std::vector<TkWord> TokenTable;
private:
	FileUtil::ReadFile file_;
	int64_t linenum_;
	std::string sourceStr_;
	std::string tkstr_;
	char ch_; 
	int32_t token_;
	int32_t tkValue_;
};

}//end namespace T
