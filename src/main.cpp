#include "core/lex.h"
#include <iostream>
using namespace T;
using namespace T::Util;

int main()
{
    std::string s = "hello T lang !";
    outputLine(s, 1, 1, 3, 4);
    output(getColorText(s, TextColor::Blue));
    return 0;
}