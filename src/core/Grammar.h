#pragma once

#include<string>
#include"Symbol.h"
#include"Lex.h"
#include"Compiler.h"

namespace T
{

class Compiler;

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
	T_MEMBER = 0x40000000, 
	T_PARAMS = 0x60000000,  
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

constexpr int ALIGNSET = 0x100;

class Grammar
{
public:
	Grammar(Compiler& compiler);
    SyntaxState getSyntaxState() const { return syntaxState_; }
    int getgetSyntaxLevel() const { return syntaxLevel_;}
    void translationUnit();
    void externalDeclaration(StorageClass level);
    //void initializer(SymbolType* ptype,  int c, Section* psec); 
	SymbolTypePointer typeSpecifier();
	SymbolPointer structSpecifier();
    void structDeclarationList(SymbolTypePointer type);
    void structDeclaration(int* maxalign, int* offset, SymbolPointer& ps);
    void declarator(SymbolTypePointer type, int* v, int* forceAlign);
    void functionCallingConvention(int* fc);
    void structMemberAlignment(int* forceAlign);
    void directDeclarator(SymbolTypePointer type, int* v, int funcCall);
    void directDeclaratorPostfix(SymbolTypePointer type,  int funcCall);
    void parameterTypeList(SymbolTypePointer type, int funcCall);
    void funcbody(SymbolPointer sym);
    int isTypeSpecifier(int id);
    void statement(int* bsym, int* csym);
    void compoundStatement(int* bsym, int* csym);
    void ifStatement(int* bsym, int* csym);
    void forStatement(int* bsym, int* csym);
    void continueStatement(int* csym);
    void breakStatement(int* bsym);
    void returnStatement();
    void expressionStatement();
    void expression();
    void assignmentExpression();
    void equalityExpression();
    void relationalExpression();
    void additiveExpression();
    void multiplicativeExpression();
    void unaryExpression();
    void sizeofExpression();
    void postfixExpression();
    void primaryExpression();
    void argumentExpressionList();
    void printTab(int num);
    void syntaxIndent();
private:
    SyntaxState syntaxState_;
    int syntaxLevel_;
	Compiler& manager_;
};

}//end namespace T

