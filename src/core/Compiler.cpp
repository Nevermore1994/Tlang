#include"Compiler.h"

using namespace T;

Compiler::Compiler(const char * path)
{
	init(path);
}

Compiler::Compiler(const std::string path)
{
	init(path.c_str());
}

void Compiler::init(const char * path)
{
	lex_ = std::make_shared<Lex>(path, *this);
	grammar_ = std::make_shared<Grammar>(*this);
	symbolManager_ = std::make_shared<SymbolManager>(*this);
}