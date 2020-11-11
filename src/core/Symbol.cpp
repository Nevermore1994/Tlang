#include"Symbol.h"
#include"Lex.h"
#include<assert.h>

using namespace T;

SymbolPointer SymbolManager::findStructDefine(uint32_t code)
{
    if(code >= Lex::TokenTable.size())
    {
        return Lex::TokenTable.at(code)->symStruct;
    }
    return nullptr;
}

SymbolPointer SymbolManager::findId(uint32_t id)
{
    if(id >= Lex::TokenTable.size())
    {
        return Lex::TokenTable.at(id)->symId;
    }
    return nullptr;
}

int SymbolManager::typeSize(const SymbolTypePointer t, int& v)
{
    return 0;
}
void SymbolManager::makePointer(SymbolTypePointer t)
{
    
}

SymbolPointer SymbolManager::symDirectPush(StackType t, int v, const SymbolTypePointer& type, int r, int c)
{
    std::stack<SymbolPointer>& stack = globalSymbols;
    if(t == StackType::LOCAL_STACK)
    {
        stack = localSymbols;
    }
    return stack.emplace(std::make_shared<Symbol>(v, type, r, c));
}

SymbolPointer SymbolManager::symPush(int v, const SymbolTypePointer& type, int r, int c)
{
    StackType t = StackType::GLOBAL_STACK;
    if(!localSymbols.empty())
    {
        t = StackType::LOCAL_STACK;
    }
    auto p = symDirectPush(t, v, type, r, c);

    if((v & T_STRUCT) || v < static_cast<int>(StorageClass::T_ANOM))
    {
        int index = v & ~T_STRUCT;
        auto tkWord = Lex::TokenTable[v & ~T_STRUCT];
    }
    return nullptr;
}