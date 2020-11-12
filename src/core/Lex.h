#pragma once

#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
#include<cctype> 
#include<algorithm>
#include"Utility.h"
#include"Compiler.h"
#include"Define.h"

namespace T
{

class Compiler;


constexpr int32_t MAXKEYSIZE = 4 * 1024;

struct Symbol;

struct TkWord
{
	TkWord(int32_t index, const std::string& s)
		: spelling(s)
		, tkcode(index)
		, symStruct(nullptr)
		, symId(nullptr)
	{
	}

	TkWord(int32_t index, const char* s)
		: spelling(s)
		, tkcode(index)
		, symStruct(nullptr)
		, symId(nullptr)
	{
	}

	TkWord(int32_t index, const std::string& s, std::shared_ptr<Symbol>& str, std::shared_ptr<Symbol>& id)
		: spelling(s)
		, tkcode(index)
		, symStruct(str)
		, symId(id)
	{

	}
	int32_t tkcode;
	std::string spelling;
	std::shared_ptr<Symbol> symStruct;
	std::shared_ptr<Symbol> symId;
};

class Lex
{
public:
	
	Lex(const std::string& path, Compiler& compiler);

	Lex(const char* path, Compiler& compiler);

	uint32_t tkWordInsert(const std::string&);

	void tkWordDirectInsert(std::shared_ptr<TkWord> w);

	int tkWordFind(const std::string& str);

	inline void getCh()
	{
		ch_ = file_.readCh();
	}

	void preprocess();
	void parseIdentifier();
	bool isVaildCharacter(char ch);
	void parseNum();
	void parseString(char sep);
	void clearParseInfo();
	void initLex();
	uint32_t handleToken();
	uint32_t getToken() const;
	void skipWhiteSpace();
	void parseComment(uint32_t type);
	std::string getTkstr(uint32_t index);
	void testLex();
	void colorToken(uint32_t lex_state);

	inline int64_t getFileLineNum() const noexcept
	{
		if (ch_ != -1)
			return -1;
		return linenum_;
	}
	
	inline const FileUtil::ReadFile& getReadFile() const
	{
		return file_;
	}
public:
	static std::unordered_map<std::string, int32_t> TokenHashTable;
	static std::vector<std::shared_ptr<TkWord>> TokenTable;
private:
	FileUtil::ReadFile file_;
	int64_t linenum_;
	std::string sourceStr_;
	std::string tkstr_;
	char ch_; 
	uint32_t token_;
	int32_t tkValue_;
	Compiler& manager_;
};

}//end namespace T
