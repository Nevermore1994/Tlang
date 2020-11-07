#include"Compiler.h"

using namespace T;

Compiler::Compiler(const char * path)
{
	lex_ = std::make_shared<Lex>(path, *this);
	grammar_ = std::make_shared<Grammar>(*this);
}

Compiler::Compiler(const std::string path)
{
	lex_ = std::make_shared<Lex>(path, *this);
	grammar_ = std::make_shared<Grammar>(*this);
}

