#pragma once

#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cstdio>
#include<cctype> 
#include "util.h"

namespace T
{
enum enum_TokenCode
{
	/* ��������ָ��� */
	TK_PLUS,							// + �Ӻ�
	TK_MINUS,							// - ����
	TK_STAR,							// * �Ǻ�
	TK_DIVIDE,							// / ����
	TK_MOD,								// % ���������
	TK_EQ,								// == ���ں�
	TK_NEQ,								// != �����ں�
	TK_LT,								// < С�ں�
	TK_LEQ,								// <= С�ڵ��ں�
	TK_GT,								// > ���ں�
	TK_GEQ,								// >= ���ڵ��ں�
	TK_ASSIGN,							// = ��ֵ����� 
	TK_POINTSTO,						// -> ָ��ṹ���Ա�����
	TK_DOT,								// . �ṹ���Ա�����
	TK_AND,								// & ��ַ�������
	TK_OPENPA,							// ( ��Բ����
	TK_CLOSEPA,							// ) ��Բ����
	TK_OPENBR,							// [ ��������
	TK_CLOSEBR,							// ] ��Բ����
	TK_BEGIN,							// { �������
	TK_END,								// } �Ҵ�����
	TK_SEMICOLON,						// ; �ֺ�    
	TK_COMMA,							// , ����
	TK_ELLIPSIS,						// ... ʡ�Ժ�
	TK_EOF,								// �ļ�������
	TK_SPACE,							//
										/* ���� */
	TK_CINT,							// ���ͳ���
	TK_CCHAR,							// �ַ�����
	TK_CSTR,							// �ַ�������

	/* �ؼ��� */
	KW_CHAR,							// char�ؼ���
	KW_SHORT,							// short�ؼ���
	KW_INT,								// int�ؼ���
	KW_VOID,							// void�ؼ���  
	KW_STRUCT,							// struct�ؼ���   
	KW_IF,								// if�ؼ���
	KW_ELSE,							// else�ؼ���
	KW_FOR,								// for�ؼ���
	KW_CONTINUE,						// continue�ؼ���
	KW_BREAK,							// break�ؼ���   
	KW_RETURN,							// return�ؼ���
	KW_SIZEOF,							// sizeof�ؼ���
	KW_INCLUDE,
	KW_DO,
	KW_END,

	KW_REQUIRE,
	KW_ALIGN,							// __align�ؼ���	
	KW_CDECL,							// __cdecl�ؼ��� standard c call
	KW_STDCALL,							// __stdcall�ؼ��� pascal c call

	/* ��ʶ�� */
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
