#include "lex.h"

namespace viac
{

bool Lex::TkWordInsert(std::string &str)
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

void Lex::TkWordDirectInsert(TkWord w)
{
	tkHashTable._Insert_or_assign(w.spelling, tkTable.size());
	tkTable.push_back(w);
}

int Lex::TkWordFind(const std::string & str)
{
	if (tkHashTable.count(str) > 0)
	{
		return tkHashTable[str];
	}
	return -1;
}

void Lex::ParseComment(int type)
{
	if (type == 1) 
	{
#pragma region parse star comment
		GetCh();
		while (true)
		{
			while (true)
			{
				if (ch == '\n' || ch == '*' || ch == File_EOF)
				{
					break;
				}
				GetCh();
			}
			if (ch == '\n')
			{
				++linenum;
				GetCh();
			}
			else if (ch == '*')
			{
				GetCh();
				if (ch == '/')
				{
					GetCh();
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
		GetCh();
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
				GetCh();
			}
		}
		GetCh();
#pragma endregion
	}
}

std::string Lex::GetTkstr(const int32_t index)
{
	if (index >= tkTable.size())
	{
		return "";
	}
	else if (index >= static_cast<int32_t>(enum_TokenCode::TK_CINT) && index <= static_cast<int32_t>(enum_TokenCode::TK_CINT))
	{
		return SourceStr;
	}
	else
	{
		return tkTable[index].spelling;
	}
}


}
