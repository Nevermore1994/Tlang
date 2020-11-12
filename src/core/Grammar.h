#pragma once

#include<string>
#include"Symbol.h"
#include"Lex.h"
#include"Compiler.h"
#include"Define.h"

namespace T
{

class Compiler;

constexpr int32_t ALIGNSET = 0x100;

class Grammar
{
public:
	Grammar(Compiler& compiler);
    inline SyntaxState getSyntaxState() const { return syntaxState_; }
    inline int32_t getSyntaxLevel() const { return syntaxLevel_;}
    void translationUnit();
    void externalDeclaration(StorageClass level);
    //void initializer(SymbolType* ptype,  int32_t c, Section* psec); 
	SymbolTypePointer typeSpecifier();
	SymbolPointer structSpecifier();
    void structDeclarationList(SymbolTypePointer type);
    void structDeclaration(int32_t& maxalign, int32_t& offset, SymbolPointer& ps);
    void declarator(SymbolTypePointer type, int32_t& v, int32_t& forceAlign);
    void functionCallingConvention(int32_t& fc);
    void structMemberAlignment(int32_t& forceAlign);
    void directDeclarator(SymbolTypePointer type, int32_t& v, int32_t funcCall);
    void directDeclaratorPostfix(SymbolTypePointer type,  int32_t funcCall);
    void parameterTypeList(SymbolTypePointer type, int32_t funcCall);
    void funcbody(SymbolPointer sym);
    int32_t isTypeSpecifier(int32_t id);
    void statement(int32_t& bsym, int32_t& csym);
    void compoundStatement(int32_t& bsym, int32_t& csym);
    void ifStatement(int32_t& bsym, int32_t& csym);
    void forStatement(int32_t& bsym, int32_t& csym);
    void continueStatement(int32_t& csym);
    void breakStatement(int32_t& bsym);
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
    void printTab(int32_t num);
    void syntaxIndent();
private:
    SyntaxState syntaxState_;
    int32_t syntaxLevel_;
	Compiler& manager_;
};

}//end namespace T

