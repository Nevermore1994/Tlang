#include "lex.h"
#include "util.h"
#if _MSC_VER
#include <windows.h>
#else
#include <stdio.h>
#endif

using namespace T::Util;

namespace T
{

bool Lex::tkWordInsert(const std::string &str)
{
    if (tkHashTable.count(str) == 0)
    {
        int index = tkTable.size();
        tkHashTable[str] = index;
        tkTable.emplace_back(str, tkTable.size());
        return true;
    }
    return false;
}

void Lex::tkWordDirectInsert(TkWord w)
{
    tkHashTable.insert_or_assign(w.spelling, tkTable.size());
    tkTable.push_back(std::move(w));
}

int32_t Lex::tkWordFind(const std::string & str)
{
    if (tkHashTable.count(str) > 0)
    {
        return tkHashTable[str];
    }
    return -1;
}

void Lex::parseComment(const int32_t type)
{
    if (type == 1)
    {
#pragma region parse star comment
        getCh();
        while (true)
        {
            while (true)
            {
                if (ch == '\n' || ch == '*' || ch == File_EOF)
                {
                    break;
                }
                getCh();
            }
            if (ch == '\n')
            {
                ++linenum;
                getCh();
            }
            else if (ch == '*')
            {
                getCh();
                if (ch == '/')
                {
                    getCh();
                    return;
                }
            }
            else
            {
                //todo hint error message
                return;
            }
        }
#pragma endregion
    }
    else
    {
#pragma region parse slash comment
        getCh();
        while (true)
        {
            if (ch == '\n')
            {
                ++linenum;
                break;
            }
            else if (ch == File_EOF)
            {
                return;
            }
            else
            {
                getCh();
            }
        }
        getCh();
#pragma endregion
    }
}

std::string Lex::getTkstr(const int32_t index)
{
    if (index >= tkTable.size())
    {
        return "";
    }
    else if (index >= static_cast<int32_t>(enum_TokenCode::TK_CINT) && index <= static_cast<int32_t>(enum_TokenCode::TK_CSTR))
    {
        return sourceStr;
    }
    else
    {
        return tkTable[index].spelling;
    }
}

void Lex::testLex()
{
    do
    {
        getToken();
        colorToken(LEX_NORMAL);
    } while (token != TK_EOF);
    outputConsoleLine("\n code line: ", linenum, " L");
}

#if _MSC_VER

void Lex::colorToken(const int32_t lex_state)
{
    HANDLE had = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (lex_state)
    {
        case LEX_NORMAL:
        {
            if (token >= TK_IDENT)
                SetConsoleTextAttribute(had, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
            else if (token >= KW_CHAR)
                SetConsoleTextAttribute(had, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            else if (token >= TK_CINT)
                SetConsoleTextAttribute(had, FOREGROUND_RED | FOREGROUND_GREEN);
            else
                SetConsoleTextAttribute(had, FOREGROUND_RED | FOREGROUND_INTENSITY);
            outputConsole(getTkstr(token));
            break;
        }
        case LEX_SEP:
        {
            outputConsole(ch);
            break;
        }
    }
}

#else
void Lex::colorToken(const int32_t lex_state)
{
    switch (lex_state)
    {
        case LEX_NORMAL:
        {
            TextColor color = TextColor::Black;
            int32_t info = nil;
            if (token >= TK_IDENT)
            {
                color = TextColor::Blue;
                info = 1;
            }
            else if (token >= KW_CHAR)
                color = TextColor::Green;
            else if (token >= TK_CINT)
                color = TextColor::Green;
            else
                color = TextColor::Red;
            outputConsole(getColorText(getTkstr(token), color, info));
            break;
        }
        case LEX_SEP:
        {
            outputConsole(ch);
            break;
        }
    }
}
#endif

void Lex::clearParseInfo()
{
    Util::clearString(sourceStr);
    Util::clearString(tkstr);
}

void Lex::initLex()
{
    tkTable = {
        {TK_PLUS,         "+"            },
        {TK_MINUS,        "-"            },
        {TK_STAR,         "*"            },
        {TK_DIVIDE,       "/"            },
        {TK_MOD,          "%"            },
        {TK_EQ,           "=="           },
        {TK_NEQ,          "!="           },
        {TK_LT,           "<"            },
        {TK_LEQ,          "<="           },
        {TK_GT,           ">"            },
        {TK_GEQ,          ">="           },
        {TK_ASSIGN,       "="            },
        {TK_POINTSTO,     "->"           },
        {TK_DOT,          "."            },
        {TK_AND,          "&"            },
        {TK_OPENPA,       "("            },
        {TK_CLOSEPA,      ")"            },
        {TK_OPENBR,       "["            },
        {TK_CLOSEBR,      "]"            },
        {TK_BEGIN,        "{"            },
        {TK_END,          "}"            },
        {TK_SEMICOLON,    ";"            },
        {TK_COMMA,        ","            },
        {TK_ELLIPSIS,     "..."          },
        {TK_EOF,          "End_Of_File\n"},
        {TK_SPACE,        "",            },
        {TK_CINT,         "int_const",   },
        {TK_CCHAR,        "char_const",  },
        {TK_CSTR,         "str_const",   },

        {KW_CHAR,         "char"         },
        {KW_SHORT,        "short"        },
        {KW_INT,          "int"          },
        {KW_VOID,         "void"         },
        {KW_STRUCT,       "struct"       },

        {KW_IF,           "if"           },
        {KW_ELSE,         "else"         },
        {KW_FOR,          "for"          },
        {KW_CONTINUE,     "continue"     },
        {KW_BREAK,        "break"        },
        {KW_RETURN,       "return"       },
        {KW_SIZEOF,       "sizeof"       },
        {KW_INCLUDE,      "include"      },
        {KW_DO,           "do"           },
        {KW_END,          "end"          },
        {KW_ALIGN,        "__align"      },
        {KW_CDECL,        "__cdecl"      },
        {KW_STDCALL,      "__stdcall"    },
    };
}

void Lex::getToken()
{
    preprocess();
    switch (ch)
    {
        case 'a':	case 'b':	case 'c':	case 'd':
        case 'e':	case 'f':	case 'g':	case 'h':
        case 'i':	case 'j':	case 'k':	case 'l':
        case 'm':	case 'n':	case 'o':	case 'p':
        case 'q':	case 'r':	case 's':	case 't':
        case 'u':	case 'v':	case 'w':	case 'x':
        case 'y':	case 'z':
        case 'A':	case 'B':	case 'C':	case 'D':
        case 'E':	case 'F':	case 'G':	case 'H':
        case 'I':	case 'J':	case 'K':	case 'L':
        case 'M':	case 'N':	case 'O':	case 'P':
        case 'Q':	case 'R':	case 'S':	case 'T':
        case 'U':	case 'V':	case 'W':	case 'X':
        case 'Y':	case 'Z':
        case '_':
        {
            parseIdentifier();
            tkWordInsert(tkstr);
            token = tkTable.back().tkcode;
            break;
        }
        case '0':	case '1':	case '2':	case '3':
        case '4':   case '5':	case '6':	case '7':
        case '8':	case '9':
        {
            parseNum();
            token = TK_CINT;
            break;
        }
        case '+':
        {
            getCh();
            token = TK_PLUS;
            break;
        }
        case '-':
        {
            getCh();
            if (ch == '>')
            {
                token = TK_POINTSTO;
                getCh();
            }
            else
                token = TK_MINUS;
            break;
        }
        case '/':
        {
            token = TK_DIVIDE;
            getCh();
            break;
        }
        case '%':
        {
            token = TK_MOD;
            getCh();
            break;
        }
        case '=':
        {
            getCh();
            if (ch == '=')
            {
                token = TK_EQ;
                getCh();
            }
            else
            {
                token = TK_ASSIGN;
            }
            break;
        }
        case '!':
        {
            getCh();
            if (ch == '=')
            {
                token = TK_NEQ;
                getCh();
            }
            else
            {
            }
            break;
        }
        case '<':
        {
            getCh();
            if (ch == '=')
            {
                token = TK_LEQ;
                getCh();
            }
            else
                token = TK_LT;
            break;
        }
        case '>':
        {
            getCh();
            if (ch == '=')
            {
                token = TK_GEQ;
                getCh();
            }
            else
                token = TK_GT;

            break;
        }
        case '.':
        {
            getCh();
            if (ch == '.')
            {
                getCh();
                if (ch == '.')
                {
                    token = TK_ELLIPSIS;
                }
                else
                {

                }
                getCh();
            }
            else
            {
                token = TK_DOT;
            }
            break;
        }
        case '&':
        {
            token = TK_AND;
            getCh();
            break;
        }
        case ';':
        {
            token = TK_SEMICOLON;
            getCh();
            break;
        }
        case '(':
        {
            token = TK_OPENPA;
            getCh();
            break;
        }
        case ')':
        {
            token = TK_CLOSEPA;
            getCh();
            break;
        }
        case '[':
        {
            token = TK_OPENBR;
            getCh();
            break;
        }
        case ']':
        {
            token = TK_CLOSEBR;
            getCh();
            break;
        }
        case '{':
        {
            token = TK_BEGIN;
            getCh();
            break;
        }
        case '}':
        {
            token = TK_END;
            getCh();
            break;
        }

        case '\n':
        {
            token = TK_SPACE;
            getCh();
            break;
        }
        case ',':
        {
            token = TK_COMMA;
            getCh();
            break;
        }
        case '*':
        {
            token = TK_STAR;
            getCh();
            break;
        }
        case '\"':
        {
            parseString(ch);
            token = TK_CSTR;
            break;
        }
        case '\'':
        {
            parseString(ch);
            token = TK_CCHAR;
            tkValue = *(tkstr.c_str());
            break;
        }
        case EOF:
        {
            token = TK_EOF;
            break;
        }
        default:
        {
            getCh();
            break;
        }
    }
}

void Lex::skipWhiteSpace()
{
    while (ch == ' ' || ch == '\t' || ch == '\r')
    {
        if (ch == '\r')
        {
            getCh();
            if (ch != '\n')
                return;
            linenum++;
        }
        else
            outputConsole(ch);  
        getCh();
    }
}

void Lex::preprocess()
{
    while (true)
    {
        if (ch == ' ' || ch == '\t' || ch == '\r')
        {
            skipWhiteSpace();
        }
        else if (ch == '/')
        {
            getCh();
            if (ch == '*')
            {
                parseComment(1);
            }
            else if (ch == '/')
            {
                parseComment(2);
            }
            else
            {
                ungetc(ch, fin);
                ch = '/';
                break;
            }
        }
        else if (ch == '#')
        {

        }
        else
        {
            break;
        }
    }
}

void Lex::parseIdentifier()
{
    sourceStr.clear();
    sourceStr.resize(0);
    sourceStr.append(1, ch);
    getCh();
    while (std::isdigit(ch) || isVaildCharacter(ch))
    {
        sourceStr.append(1, ch);
        getCh();
    }
}

bool Lex::isVaildCharacter(const char& ch)
{
    return std::isalpha(ch) || (ch == '_');
}

void Lex::parseNum()
{
    clearParseInfo();

    do
    {
        tkstr.append(1, ch);
        sourceStr.append(1, ch);
        getCh();
    } while (std::isdigit(ch));

    if (ch == '.')
    {
        do
        {
            tkstr.append(1, ch);
            sourceStr.append(1, ch);
            getCh();
        } while (std::isdigit(ch));
    }
    tkValue = std::stoi(tkstr);
}

void Lex::parseString(const char & sep)
{
    char c;
    clearParseInfo();

    sourceStr.append(1, sep);
    getCh();
    while (true)
    {
        if (ch == sep)
            break;
        else if (ch == '\\')
        {
            sourceStr.append(1, ch);
            getCh();
            switch (ch)
            {
                case '0':
                    c = '\0';
                    break;
                case 'a':
                    c = '\a';
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 'v':
                    c = '\v';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case '\"':
                    c = '\"';
                    break;
                case '\'':
                    c = '\'';
                    break;
                case '\\':
                    c = '\\';
                    break;
                default:
                    c = ch;
                    if (c >= '!' && c <= '~')
                    {
                    }
                    else
                    {
                    }
                    break;
            }
            tkstr.append(1, c);
            sourceStr.append(1, ch);
            getCh();
        }
        else
        {
            tkstr.append(1, ch);
            sourceStr.append(1, ch);
            getCh();
        }
    }
    sourceStr.append(1, sep);
    getCh();
}

}