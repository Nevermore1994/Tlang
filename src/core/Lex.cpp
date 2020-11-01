#include "Lex.h"
#if _MSC_VER
#include <windows.h>
#else
#include <stdio.h>
#endif

using namespace T::Util;
using namespace T::FileUtil;

namespace T
{

std::unordered_map<std::string, int32_t> Lex::TokenHashTable;
std::vector<TkWord> Lex::TokenTable;

Lex::Lex(const std::string& path):file_(path),ch_(0), linenum_(0)
{
    initLex();
    getCh();
}

Lex::Lex(const char* path):file_(path), ch_(0), linenum_(0)
{
    initLex();
    getCh();
}

uint32_t Lex::tkWordInsert(const std::string &str)
{
    if (TokenHashTable.count(str) == 0)
    {
        TokenHashTable[str] = TokenTable.size();
        TokenTable.emplace_back(TokenTable.size(), str);
    }
	return TokenHashTable[str];
}

void Lex::tkWordDirectInsert(TkWord w)
{
    TokenHashTable.insert_or_assign(w.spelling, TokenTable.size());
    TokenTable.push_back(std::move(w));
}

int32_t Lex::tkWordFind(const std::string & str)
{
    if (TokenHashTable.count(str) > 0)
    {
        return TokenHashTable[str];
    }
    return -1;
}

void Lex::parseComment(int32_t type)
{
    if (type == 1)
    {
#pragma region parse star comment
        getCh();
        while (true)
        {
            while (true)
            {
                if (ch_ == '\n' || ch_ == '*' || ch_ == EOF)
                {
                    break;
                }
                getCh();
            }
            if (ch_ == '\n')
            {
                ++linenum_;
                getCh();
            }
            else if (ch_ == '*')
            {
                getCh();
                if (ch_ == '/')
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
            if (ch_ == '\n')
            {
                ++linenum_;
                break;
            }
            else if (ch_ == EOF)
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

std::string Lex::getTkstr(int32_t index)
{
    if (index >= TokenTable.size())
    {
        return "";
    }
    else if (index >= static_cast<int32_t>(TK_CINT) && index <= static_cast<int32_t>(TK_FLOAT))
    {
        return sourceStr_;
    }
    else
    {
        return TokenTable[index].spelling;
    }
}

void Lex::testLex()
{
    do
    {
        getToken();
        colorToken(LEX_NORMAL);
    } while (token_ != TK_EOF);
    outputConsoleLine("\n code line: ", linenum_, " L");
}

#if _WIN32

void Lex::colorToken(int32_t lex_state)
{
    HANDLE had = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (lex_state)
    {
        case LEX_NORMAL:
        {
            if (token_ >= TK_IDENT)
                SetConsoleTextAttribute(had, FOREGROUND_INTENSITY);
            else if (token_ >= KW_CHAR)
                SetConsoleTextAttribute(had, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            else if (token_ >= TK_CINT)
                SetConsoleTextAttribute(had, FOREGROUND_RED | FOREGROUND_GREEN);
            else
                SetConsoleTextAttribute(had, FOREGROUND_RED | FOREGROUND_INTENSITY);
            outputConsole(getTkstr(token_));
            break;
        }
        case LEX_SEP:
        {
            outputConsole(ch_);
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
            if (token_ >= TK_IDENT)
            {
                color = TextColor::Blue;
                info = 1;
            }
            else if (token_ >= KW_CHAR)
                color = TextColor::Green;
            else if (token_ >= TK_CINT)
                color = TextColor::Green;
            else
                color = TextColor::Red;
            //outputConsole(getColorText(getTkstr(token_), color, info));
            outputConsole(getTkstr(token_));
            break;
        }
        case LEX_SEP:
        {
            outputConsole(ch_);
            break;
        }
    }
}
#endif

void Lex::clearParseInfo()
{
    Util::clearString(sourceStr_);
    Util::clearString(tkstr_);
}

void Lex::initLex()
{
    TokenTable = {
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
        {TK_EOF,          "\n"           },
        {TK_SPACE,        "",            },
        {TK_CINT,         "int_const",   },
        {TK_CCHAR,        "char_const",  },
        {TK_CSTR,         "str_const",   },
		{TK_DOUBLE,       "double_const",},
		{TK_FLOAT,        "float_const", },

        {KW_CHAR,         "char"         },
        {KW_SHORT,        "short"        },
        {KW_INT,          "int"          },
        {KW_VOID,         "void"         },
        {KW_DOUBLE,       "double"       },
        {KW_FLOAT,        "float"        },
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
		{KW_LET,          "let"          },
        {KW_ALIGN,        "__align"      },
        {KW_CDECL,        "__cdecl"      },
        {KW_STDCALL,      "__stdcall"    },
    };
	for (auto& word : TokenTable) 
	{
		TokenHashTable[word.spelling] = word.tkcode;
	}
}

void Lex::getToken()
{
    preprocess();
    if(isVaildCharacter(ch_))
    {
        parseIdentifier();
		token_ = tkWordInsert(tkstr_);
        return;
    }
    else if(std::isdigit(ch_))
    {
        parseNum();
        return;
    } 
    else if(ch_ == EOF)
    {
        token_ = TK_EOF;
        return;
    }
    else
    {
        switch (ch_)
        {
            case '+':
                token_ = TK_PLUS;
                break;
            case '-':
                getCh();
                if (ch_ == '>')
                {
                    token_ = TK_POINTSTO;
                }
                else
                    token_ = TK_MINUS;
                break;
            case '/':
                token_ = TK_DIVIDE;
                break;
            case '%':
                token_ = TK_MOD;
                break;
            case '=':
                getCh();
                if (ch_ == '=')
                {
                    token_ = TK_EQ;
                }
                else
                    token_ = TK_ASSIGN;
                break;
            case '!':
                getCh();
                if (ch_ == '=')
                    token_ = TK_NEQ;
                else
                {
                    //Negation
                }
                break;
            case '<':
                getCh();
                if (ch_ == '=')
                    token_ = TK_LEQ;
                else
                    token_ = TK_LT;
                break;
            case '>':
                getCh();
                if (ch_ == '=')
                    token_ = TK_GEQ;
                else
                    token_ = TK_GT;
                break;
            case '.':
                getCh();
                if (ch_ == '.')
                {
                    getCh();
                    if (ch_ == '.')
                        token_ = TK_ELLIPSIS;
                    else
                        outputConsoleLine("error, not suppprt sign");
                }
                else
                    token_ = TK_DOT;
                break;
            case '&':
                token_ = TK_AND;
                break;
            case ';':
                token_ = TK_SEMICOLON;
                break;
            case '(':
                token_ = TK_OPENPA;
                break;
            case ')':
                token_ = TK_CLOSEPA;
                break;
            case '[':
                token_ = TK_OPENBR;
                break;
            case ']':
                token_ = TK_CLOSEBR;
                break;
            case '{':
                token_ = TK_BEGIN;
                break;
            case '}':
                token_ = TK_END;
                break;
            case '\n':
                token_ = TK_SPACE;
                break;
            case ',':
                token_ = TK_COMMA;
                break;
            case '*':
                token_ = TK_STAR;
                break;
            case '\"':
                parseString(ch_);
                token_ = TK_CSTR;
                break;
            case '\'':
                parseString(ch_);
                token_ = TK_CCHAR;
                tkValue_ = *(tkstr_.c_str());
                break;
            default:
                break;
        }
    }
    getCh();
}

void Lex::skipWhiteSpace()
{
    while (ch_ == ' ' || ch_ == '\t' || ch_ == '\r')
    {
        if (ch_ == '\r')
        {
            getCh();
            if (ch_ != '\n')
                return;
            linenum_++;
			outputConsole(ch_);
        }
        else
            outputConsole(ch_);  
        getCh();
    }
}

void Lex::preprocess()
{
    while (true)
    {
        if (ch_ == ' ' || ch_ == '\t' || ch_ == '\r')
        {
            skipWhiteSpace();
        }
        else if (ch_ == '/')
        {
            getCh();
            if (ch_ == '*')
            {
                parseComment(1);
            }
            else if (ch_ == '/')
            {
                parseComment(2);
            }
            else
            {
                file_.backfillCh(ch_);
                ch_ = '/';
                break;
            }
        }
        else if (ch_ == '#')
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
    tkstr_.clear();
    tkstr_.resize(0);
    while (std::isdigit(ch_) || isVaildCharacter(ch_))
    {
        tkstr_.push_back(ch_);
        getCh();
    }
}

bool Lex::isVaildCharacter(char ch)
{
    return std::isalpha(ch) || (ch == '_');
}

void Lex::parseNum()
{
    clearParseInfo();
    token_ = TK_CINT;
    do
    {
        tkstr_.push_back(ch_);
        sourceStr_.push_back(ch_);
        getCh();
    } while (std::isdigit(ch_));

    if (ch_ == '.')
    {
        do
        {
            tkstr_.push_back(ch_);
            sourceStr_.push_back(ch_);
            getCh();
        } while (std::isdigit(ch_));
        token_ = TK_DOUBLE; 
        if(ch_ == 'f')
        {
           token_ = TK_FLOAT; 
        }
    }
    tkValue_ = std::stoi(tkstr_);
}

void Lex::parseString(char sep)
{
    char c;
    clearParseInfo();

    sourceStr_.push_back(sep);
    getCh();
    while (true)
    {
        if (ch_ == sep)
            break;
        else if (ch_ == '\\')
        {
            sourceStr_.push_back(ch_);
            getCh();
            switch (ch_)
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
                    c = ch_;
                    if (c >= '!' && c <= '~')
                    {
                    }
                    else
                    {
                    }
                    break;
            }
            tkstr_.push_back(c);
            sourceStr_.push_back(ch_);
            getCh();
        }
        else
        {
            tkstr_.push_back(ch_);
            sourceStr_.push_back(ch_);
            getCh();
        }
    }
    sourceStr_.push_back(sep);
    getCh();
}

}