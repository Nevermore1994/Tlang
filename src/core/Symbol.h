#pragma once

#include<stack>
#include<memory>

namespace T
{

struct Symbol;

using SymbolPoint = std::shared_ptr<Symbol>;

struct SymbolType
{
    int type;
    SymbolPoint ref;
};

struct Symbol
{
	int code;  // symbol code
	int reg;  //symbol register
	int value; //symbol link value 
    SymbolType type; 
	SymbolPoint next; // next pointer
	SymbolPoint pre; // forward pointer
};


class SymbolManager
{
public:
    SymbolPoint findDefine(int code);
    SymbolPoint findId(int id);
    int typeSize(const SymbolType* t, int* v);
    void makePointer(SymbolType* t);
public:
    static std::stack<SymbolPoint> globalSymbols;
    static std::stack<SymbolPoint> localSymbols;
};

}// end namespace T

