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
