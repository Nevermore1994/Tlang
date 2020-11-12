#include"Grammar.h"
#include"Symbol.h"

using namespace T; 

Grammar::Grammar(Compiler & compiler)
	:manager_(compiler)
{
}

void Grammar::translationUnit()
{
	while(manager_.getLex()->getToken() != TK_EOF)
	{
		externalDeclaration(StorageClass::T_GLOBAL);
	}
}

void Grammar::externalDeclaration(StorageClass level)
{

}

SymbolTypePointer Grammar::typeSpecifier()
{
	SymbolTypePointer res;
	TypeCode t = TYPE_UNDEFINED;
	int32_t token = manager_.getLex()->getToken();
	SymbolPointer ref;
	switch (token)
	{
		case KW_CHAR:
			t = TYPE_CHAR;
			break;
		case KW_SHORT:
			t = TYPE_SHORT;
			break;
		case KW_VOID:
			t = TYPE_VOID;
			break;
		case KW_INT:
			t = TYPE_INT;
			break;
		case KW_DOUBLE:
			t = TYPE_DOUBLE;
			break;
		case KW_FLOAT:
			t = TYPE_FLOAT;
			break;
		case KW_STRUCT:
			t = TYPE_STRUCT;
			ref = structSpecifier();
			break;
	}
	if (t != TYPE_UNDEFINED)
	{
		if (ref == nullptr)
			res = std::make_shared<SymbolType>(t);
		else
			res = std::make_shared<SymbolType>(t, ref);
		syntaxState_ = SyntaxState::SNTX_SP;
	}
	if (token != KW_STRUCT)
	{
		manager_.getLex()->handleToken();
	}
	return res;
}

SymbolPointer Grammar::structSpecifier()
{
	manager_.getLex()->handleToken();
	int32_t token = manager_.getLex()->getToken();
	
	syntaxState_ = SyntaxState::SNTX_DELAY;
	manager_.getLex()->handleToken();

	if(token == TK_BEGIN)
		syntaxState_ = SyntaxState::SNTX_LF_HT;
	else if(token == TK_CLOSEPA)
		syntaxState_ = SyntaxState::SNTX_NUL;
	else
		syntaxState_ = SyntaxState::SNTX_SP;
	
	syntaxIndent();

	if(token < TK_IDENT)
	{
		//hint struct id expect
	}
	SymbolPointer sym = manager_.getSymbolManager()->findStructDefine(token);
	auto type = std::make_shared<SymbolType>(KW_STRUCT);
	if(sym == nullptr){
		sym = manager_.getSymbolManager()->symPush(token | T_STRUCT, type, 0, -1);
	}
	return nullptr;
}

void Grammar::structDeclarationList(SymbolTypePointer type)
{
	syntaxState_ = SyntaxState::SNTX_LF_HT;
	++syntaxLevel_;
	auto psym  = type->ref;
	if(psym->code != -1)
	{
		// hint error
		return;
	}

	manager_.getLex()->handleToken();
	int32_t maxAlign = 1;
	SymbolPointer& next = psym->next;
	int32_t offset = 0;
	int32_t token = manager_.getLex()->getToken();
	while(token != TK_END)
	{
		structDeclaration(maxAlign, offset, next);
	}
	
	token = manager_.getLex()->getToken();
	// if(token == TK_END)
	// 	//skip
	syntaxState_ = SyntaxState::SNTX_LF_HT;
	//psym->code =
}

void Grammar::structDeclaration(int32_t& maxalign, int32_t& offset, SymbolPointer& ps)
{
}

void Grammar::declarator(SymbolTypePointer type, int32_t& v, int32_t& forceAlign)
{
}

void Grammar::functionCallingConvention(int32_t& fc)
{
}

void Grammar::structMemberAlignment(int32_t& forceAlign)
{
}

void Grammar::directDeclarator(SymbolTypePointer type, int32_t& v, int32_t funcCall)
{
}

void Grammar::directDeclaratorPostfix(SymbolTypePointer type, int32_t funcCall)
{
}

void Grammar::parameterTypeList(SymbolTypePointer type, int32_t funcCall)
{
}

void Grammar::funcbody(SymbolPointer sym)
{
}

int32_t Grammar::isTypeSpecifier(int32_t id)
{
	return 0;
}

void Grammar::statement(int32_t&  bsym, int32_t& csym)
{
}

void Grammar::compoundStatement(int32_t& bsym, int32_t& csym)
{
}

void Grammar::ifStatement(int32_t& bsym, int32_t& csym)
{
}

void Grammar::forStatement(int32_t& bsym, int32_t& csym)
{
}

void Grammar::continueStatement(int32_t& csym)
{
}

void Grammar::breakStatement(int32_t& bsym)
{
}

void Grammar::returnStatement()
{
}

void Grammar::expressionStatement()
{
}

void Grammar::expression()
{
}

void Grammar::assignmentExpression()
{
}

void Grammar::equalityExpression()
{
}

void Grammar::relationalExpression()
{
}

void Grammar::additiveExpression()
{
}

void Grammar::multiplicativeExpression()
{
}

void Grammar::unaryExpression()
{
}

void Grammar::sizeofExpression()
{
}

void Grammar::postfixExpression()
{
}

void Grammar::primaryExpression()
{
}

void Grammar::argumentExpressionList()
{
}

void Grammar::printTab(int32_t num)
{
}

void Grammar::syntaxIndent()
{
}

