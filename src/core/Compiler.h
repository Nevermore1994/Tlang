#pragma once

#include "Utility.h"
#include "Lex.h"
#include "Grammar.h"

namespace T
{

class Lex;
class Grammar;

class Compiler
{
public:
	Compiler(const char* path);
	Compiler(const std::string path);
	std::shared_ptr<Lex> getLex() { return lex_; }
	std::shared_ptr<Grammar> getGrammar() { return grammar_; }
	std::shared_ptr<SymbolManager> getSymbolManager() { return symbolManager_; }
private:
	void init(const char * path);
private:
	std::shared_ptr<Lex> lex_;
	std::shared_ptr<Grammar> grammar_;
	std::shared_ptr<SymbolManager> symbolManager_;
};


}//end namespace T
