#pragma once

enum TokenCode
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
	TK_DOUBLE,
	TK_FLOAT,

	KW_CHAR,							// char
	KW_SHORT,							// short
	KW_INT,								// int
	KW_VOID,							// void
	KW_DOUBLE,
	KW_FLOAT,
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
	KW_LET,

	KW_REQUIRE,
	KW_ALIGN,							// __align
	KW_CDECL,							// __cdecl  call
	KW_STDCALL,							// __stdcall

	TK_DEFAULT,
	TK_IDENT
};

enum LexStatus
{
	LEX_NORMAL,
	LEX_SEP,
};


enum class SyntaxState
{
	SNTX_NUL,
	SNTX_SP,
	SNTX_LF_HT,
	SNTX_DELAY
};

enum class StorageClass
{
	T_GLOBAL = 0x00f0,
	T_LOCAL = 0x00f1,
	T_LLOCAL = 0x00f2,
	T_CMP = 0x00f3,
	T_VALMASK = 0x00ff, //mask code
	T_LVAL = 0x0100,//lvalue
	T_SYM = 0x0200,//sign

	T_ANOM = 0x10000000,  //anonymous sign
	T_STRUCT = 0x20000000, 
	T_PARAMS = 0x60000000,  
	T_MEMBER = 0x40000000, 
};

enum TypeCode
{
	T_INT = 0,
	T_CHAR = 1,
	T_SHORT = 2,
	T_FLOAT = 3,
	T_DOUBLE = 4,
	T_VOID = 5,
	T_PTR = 6,
	T_FUNC = 7,
	T_STRUCT = 8,

	T_BTYPE = 0x000f,
	T_ARRAY = 0x0010,
	T_UNDEFINED = 0xffff,
};
