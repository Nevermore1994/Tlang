#include "core/Lex.h"
#include <iostream>
using namespace T;
using namespace T::Util;

int main()
{
    std::string s = "hello T lang !";
    outputConsole(s, 1, 1, 3, 4);
    outputConsole(getColorText(s, TextColor::Blue));
    return 0;
}