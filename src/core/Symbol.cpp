#include"Symbol.h"
#include"Lex.h"
#include<assert.h>

using namespace T;

SymbolManager::SymbolManager(Compiler& m)
    :manager_(m)
{
    
}

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

SymbolPointer SymbolManager::symDirectPush(StackType t, int32_t v, const SymbolTypePointer& type, int32_t r, int32_t c)
{
    std::stack<SymbolPointer>& stack = globalSymbols;
    if(t == StackType::LOCAL_STACK)
    {
        stack = localSymbols;
    }
    return stack.emplace(std::make_shared<Symbol>(v, type, r, c));
}

SymbolPointer SymbolManager::symPush(int32_t v, const SymbolTypePointer& type, int32_t r, int32_t c)
{
    StackType t = StackType::GLOBAL_STACK;
    if(!localSymbols.empty())
    {
        t = StackType::LOCAL_STACK;
    }
    auto p = symDirectPush(t, v, type, r, c);

    if((v & T_STRUCT) || v < static_cast<int32_t>(T_ANOM))
    {
        int32_t va = v & ~T_STRUCT;
        if(va >= Lex::TokenTable.size())
        {
            //hint error
        }
        auto tkWord = Lex::TokenTable[va];
        if(v & T_STRUCT)
            p->pre = tkWord->symStruct;
        else
            p->pre = tkWord->symId;
    }
    return p;
}

SymbolPointer SymbolManager::funcSymPush(int32_t v, const SymbolTypePointer& type)
{
    SymbolPointer p = symDirectPush(StackType::GLOBAL_STACK, v, type, 0, 0);
    auto pSym = Lex::TokenTable[v]->symId;
    while (pSym && pSym->pre)
    {
        pSym = pSym->pre;
    }
    if(pSym)
        pSym->pre = p;
    return p;
}

SymbolPointer SymbolManager::varSymPut(const SymbolTypePointer& type, int32_t t, int32_t v, int32_t addr)
{
    SymbolPointer p = nullptr;
    if((t & T_VALMASK) == T_LLOCAL)
    {
        p = symPush(v, type, t, addr);
    }
    else if(v && (t & T_VALMASK) == T_GLOBAL)
    {
        p = findId(v);
        if(p)
        {
            //hint error, define conflict
        }
        else
        {
            p = symPush(v, type, t | T_SYM, 0);
        }
    }
    return p;
}

SymbolPointer SymbolManager::secSymPut(const char* sec, int32_t c)
{
    uint32_t index = manager_.getLex()->tkWordInsert(std::string(sec));
    auto p = std::make_shared<SymbolType>(TYPE_INT);
    return symPush(Lex::TokenTable[index]->tkcode, p, T_GLOBAL, c);
}

//p = nullptr is allow
void SymbolManager::symPop(StackType t, const SymbolPointer end)
{
    std::stack<SymbolPointer>& stack = globalSymbols;
    if(t == StackType::LOCAL_STACK)
    {
        stack = localSymbols;
    }

    if(stack.empty())
        return;
    
    auto p = stack.top();
    int v = 0;
    while(p != end)
    {
        v = p->value;
        if ((v & T_STRUCT) || v < T_ANOM)
		{
            int32_t va = v & ~T_STRUCT;
            if(va >= Lex::TokenTable.size())
            {
                //hint error
            }
			auto ts = Lex::TokenTable[va];
			if (v & T_STRUCT)
				ts->symStruct = p->pre;
			else
				ts->symId = p->pre;
		}
		stack.pop();
        if(stack.empty())
            break;
		p = stack.top();
    }
}

void SymbolManager::mkPointer(SymbolTypePointer& ptype)
{
	auto psym = symPush(T_ANOM, ptype, 0, -1);
	ptype->type = TYPE_PTR; 
	ptype->ref = psym;
}


int32_t SymbolManager::typeSize(const SymbolTypePointer& t, int32_t& v)
{
    constexpr int32_t ptrSize = sizeof(nullptr);
    
    int bt = t->type & TYPE_BTYPE;
    switch (bt)
    {
        case TYPE_STRUCT:
            v = t->ref->reg;
            return t->ref->code;
        case TYPE_PTR:
            if(t->type & TYPE_ARRAY)
            {
                auto p = std::make_shared<SymbolType>(t->ref->type.type, t->ref->type.ref);
                return typeSize(p, v) * t->ref->code;
            }
            else
                return ptrSize;
        case TYPE_CHAR:
            v = 1; 
            return v;
        case TYPE_SHORT:
            v = 2;
            return v;
        case TYPE_INT:
            v = 4;
            return v;
        case TYPE_FLOAT:
            v = 4;
            return v;
        case TYPE_DOUBLE:
            v = 8;
            return v;
        default:
            v = 1;
            return v;
    }
    return 0; //void
}
