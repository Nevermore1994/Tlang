#pragma once

#include<stack>
#include<memory>

namespace T
{

struct Symbol;
struct SymbolType;

using SymbolPointer = std::shared_ptr<Symbol>;
using SymbolTypePointer = std::shared_ptr<SymbolType>;

struct SymbolType
{
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
};


class SymbolManager
{
public:
    SymbolPointer findDefine(int code);
    SymbolPointer findId(int id);
    int typeSize(const SymbolType* t, int* v);
    void makePointer(SymbolType* t);
public:
    static std::stack<SymbolPointer> globalSymbols;
    static std::stack<SymbolPointer> localSymbols;
};

}// end namespace T

