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
	SymbolType(int32_t t)
		:type(t)
		,ref(nullptr) 
	{

	}
	SymbolType(int32_t t, SymbolPointer r)
		:type(t)
		, ref(r)
	{

	}
	SymbolType()
		:type(0)
		,ref(nullptr) 
	{

	}
    int32_t type;
    SymbolPointer ref;
};

struct Symbol
{
	int32_t code;  // symbol code
	int32_t reg;  //symbol register
	int32_t value; //symbol link value 
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

	Symbol(int32_t v, const SymbolTypePointer& e, int32_t r, int32_t c)
		:code(c)
		,reg(r)
		,value(v)
		,type(e->type, e->ref)
		,next(nullptr)
		,pre(nullptr)
	{

	}
};

class Compiler;

class SymbolManager
{
public:
	enum class StackType
	{
		LOCAL_STACK = 0,
		GLOBAL_STACK,
	};
public:
	SymbolManager(Compiler& m);
    SymbolPointer findStructDefine(uint32_t code);
    SymbolPointer findId(uint32_t id);
    int32_t typeSize(const SymbolTypePointer& t, int32_t& v);
	SymbolPointer symPush(int32_t v, const SymbolTypePointer& type, int32_t r, int32_t c);
	SymbolPointer symDirectPush(StackType t, int32_t v, const SymbolTypePointer& type, int32_t r, int32_t c);
	SymbolPointer funcSymPush(int32_t v, const SymbolTypePointer& type);
	SymbolPointer varSymPut(const SymbolTypePointer& type, int32_t t, int32_t v, int32_t addr);
	SymbolPointer secSymPut(const char* sec, int32_t c);
	void symPop(StackType t, const SymbolPointer b);
	void mkPointer(SymbolTypePointer& ptype);
private:
    static std::stack<SymbolPointer> globalSymbols;
    static std::stack<SymbolPointer> localSymbols;
	Compiler& manager_;
};

}// end namespace T

