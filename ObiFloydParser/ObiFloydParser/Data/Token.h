#pragma once

#include "Utility.h"

class Token
{
public:
	unsigned long ID;
	std::string Title;
	std::string Value;
	Token* Sibling;
	Token* Alternate;
	std::vector<Token*> Children;
	int Selection;
	bool Repeat;

	Token();
	Token(std::string, std::string);
	Token(std::string, bool);
	Token(std::vector<std::string>);
	~Token();

	void Dispose();

	std::vector<Token*> GetSiblings();
	std::string ToString();
};

