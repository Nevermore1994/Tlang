#pragma once
#include <stdio.h>
#include"Compiler.h"

namespace T
{

class Compiler;

class Exception
{

public:
    Exception(Compiler& m);
    void error(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void expect(const char* fmt, ...);
    void linkError(const char* fmt, ...);
    void skip(int32_t c);
private:
    void handleException(WorkStage stage, ErrorLevel level, const char* fmt, va_list ap);
private:
    Compiler& manager_;
};

    
}//end namespace
