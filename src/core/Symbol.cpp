#include"Symbol.h"
#include"Lex.h"

using namespace T;

SymbolPointer SymbolManager::findDefine(int code)
{
    if(code >= Lex::tkTable.size())
    {
        return Lex::tkTable.at(code).symStruct;
    }
    return nullptr;
}

SymbolPointer SymbolManager::findId(int id)
{
    if(id >= Lex::tkTable.size())
    {
        return Lex::tkTable.at(id).symId;
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