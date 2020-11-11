#pragma once

#include<stack>
#include<memory>
#include"Define.h"

namespace T
{

struct Symbol;
struct SymbolType;

using SymbolPointer = std::shared_ptr<Symbol>;
using SymbolTypePointer = std::shared_ptr<SymbolType>;

struct SymbolType
{
	SymbolType(int t)
		:type(t)
		,ref(nullptr) 
	{

	}
	SymbolType(int t, SymbolPointer r)
		:type(t)
		, ref(r)
	{

	}
	SymbolType()
		:type(0)
		,ref(nullptr) 
	{

	}
    int type;
    SymbolPointer ref;
};

struct Symbol
{
	int code;  // symbol code
	int reg;  //symbol register
	int value; //symbol link value 
    SymbolType type; 
	SymbolPointer next; // next pointer
	SymbolPointer pre; // forward pointer
	
	Symbol()
		:code(0)
		,reg(0)
		,value(0)
		,type()
		,next(nullptr)
		,pre(nullptr)
	{

	}

	Symbol(int v, const SymbolTypePointer& e, int r, int c)
		:code(c)
		,reg(r)
		,value(v)
		,type(e->type, e->ref)
		,next(nullptr)
		,pre(nullptr)
	{

	}
};


class SymbolManager
{
public:
	enum class StackType
	{
		LOCAL_STACK = 0,
		GLOBAL_STACK,
	};
public:
    SymbolPointer findStructDefine(uint32_t code);
    SymbolPointer findId(uint32_t id);
    int typeSize(const SymbolTypePointer t, int& v);
    void makePointer(SymbolTypePointer t);
	SymbolPointer symPush(int v, const SymbolTypePointer& type, int r, int c);
	SymbolPointer symDirectPush(StackType t, int v, const SymbolTypePointer& type, int r, int c);
private:
    static std::stack<SymbolPointer> globalSymbols;
    static std::stack<SymbolPointer> localSymbols;
};

}// end namespace T

