#pragma once

#include <sstream>
#include <iostream>
#include <map>
#include "Token.h"
#include "Tokenizer.h"
#include "Utility.h"
#include "ParseResult.h"

class Grammer
{
private:
	static Token* head;
	static size_t furthest;
	static std::string expected;
	static std::vector<std::string> lines;

public:
	static void ReadGrammer(std::string);
	static ParseResult ParseText(std::string);
	static std::string GetParseString(std::string, bool);
	static std::string GetParseStructure();
	static void Cleanup();

private:
	static std::string GetParseStructure(Token*, int);
	static ParseResult ParseText(std::string, Token*, Token*, ParseResult);
	static void SetSiblingsChildren(Token*);
	static void AddRepeatSibling(Token*, size_t);
};

