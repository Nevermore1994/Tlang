#pragma once

#include<string>
#include"Symbol.h"
#include"Lex.h"
#include"Compiler.h"
#include"Define.h"

namespace T
{

class Compiler;

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
    int32_t syntaxLevel_;
	Compiler& manager_;
};

}//end namespace T

