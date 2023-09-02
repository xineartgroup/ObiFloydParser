#pragma once
#include "Utility.h"
#include "Token.h"

class Tokenizer
{
public:
	static Token* GetToken(std::string);
	static std::string GetTokenTitle(std::string);

private:
	static Token* GetChild(std::string);
};

