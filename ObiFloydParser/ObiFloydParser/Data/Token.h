#pragma once

#include <string>
#include <vector>

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
	Token(std::string, std::string, int);
	Token(Token*);
	Token(std::string, bool);
	Token(std::vector<std::string>);
	~Token();

	void Dispose();

	std::vector<Token*> GetSiblings();
	std::string ToString();
	static Token* GetSiblingCopy(Token*);
	static std::vector<Token*> GetChildrenCopy(Token*);
};

