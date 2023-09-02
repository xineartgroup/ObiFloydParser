#include "Token.h"

Token::Token()
{
	ID = Utility::NewID();
	Title = "";
	Value = "";
	Sibling = nullptr;
	Alternate = nullptr;
	Selection = -1;
	Repeat = false;
}

Token::Token(std::string title, std::string value)
{
	ID = Utility::NewID();
	Title = title;
	Value = value;
	Sibling = nullptr;
	Alternate = nullptr;
	Selection = -1;
	Repeat = false;
}

Token::Token(std::string title, bool encase)
{
	ID = Utility::NewID();

	if (encase && title.size() > 0 && title[0] != '<')
		title = '<' + title;
	if (encase && title.size() > 0 && title[title.size() - 1] != '>')
		title = title + '>';

	Title = title;
	Value = "";
	Sibling = nullptr;
	Selection = -1;
	Repeat = false;
}

Token::Token(std::vector<std::string> siblings)
{
	ID = Utility::NewID();
	if (siblings.size() == 0)
	{
		Title = "";
		Value = "";
		Sibling = nullptr;
		Selection = -1;
		Repeat = false;
	}
	else
	{
		Token* token = new Token(siblings[siblings.size() - 1], true);
		siblings.erase(siblings.end() - 1);
		while (siblings.size() > 0)
		{
			Token* sibling = new Token(siblings[siblings.size() - 1], true);
			siblings.erase(siblings.end() - 1);
			sibling->Sibling = token;
			token = sibling;
		}
		Title = token->Title;
		Value = token->Value;
		Sibling = token->Sibling;
		Selection = token->Selection;
		Repeat = token->Repeat;
		Children = token->Children;
	}
}

Token::~Token()
{
}

void Token::Dispose()
{
	if (Sibling)
	{
		Sibling->Dispose();
		delete Sibling;
	}
	if (Alternate)
	{
		Alternate->Dispose();
		delete Alternate;
	}
	for (size_t i = 0; i < Children.size(); i++)
	{
		if (Children[i])
		{
			Children[i]->Dispose();
			delete Children[i];
		}
	}
}

std::vector<Token*> Token::GetSiblings()
{
	std::vector<Token*> result;

	result.push_back(this);
	Token* sibling = this->Sibling;

	while (sibling)
	{
		result.push_back(sibling);
		sibling = sibling->Sibling;
	}

	return result;
}

std::string Token::ToString()
{
	if (Sibling)
		return Title + Sibling->ToString();
	return Title;
}