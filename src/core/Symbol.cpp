#include"Symbol.h"
#include"Lex.h"

using namespace T;

SymbolPointer SymbolManager::findDefine(uint32_t code)
{
    if(code >= Lex::TokenTable.size())
    {
        return Lex::TokenTable.at(code).symStruct;
    }
    return nullptr;
}

SymbolPointer SymbolManager::findId(uint32_t id)
{
    if(id >= Lex::TokenTable.size())
    {
        return Lex::TokenTable.at(id).symId;
    }
    return nullptr;
}

int SymbolManager::typeSize(const SymbolType* t, int* v)
{
    return 0;
}
void SymbolManager::makePointer(SymbolType* t)
{
    
}