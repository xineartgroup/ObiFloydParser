#include "Token.h"
#include "Utility.h"

Token::Token()
{
	ID = Utility::NewID();
	Title = "";
	Value = "";
	Sibling = NULL;
	Alternate = NULL;
	Selection = -1;
	Repeat = false;
}

Token::Token(std::string title, std::string value)
{
	ID = Utility::NewID();
	Title = title;
	Value = value;
	Sibling = NULL;
	Alternate = NULL;
	Selection = -1;
	Repeat = false;
}

Token::Token(std::string title, std::string value, int selection)
{
	ID = Utility::NewID();
	Title = title;
	Value = value;
	Sibling = NULL;
	Alternate = NULL;
	Selection = selection;
	Repeat = false;
}

Token::Token(Token* token)
{
	ID = Utility::NewID();
	if (token)
	{
		Title = token->Title;
		Value = token->Value;
		Sibling = GetSiblingCopy(token->Sibling);
		Alternate = GetSiblingCopy(token->Alternate);
		Selection = token->Selection;
		Repeat = token->Repeat;
		Children = GetChildrenCopy(token);
	}
	else
	{
		Title = "";
		Value = "";
		Sibling = NULL;
		Alternate = NULL;
		Selection = -1;
		Repeat = false;
	}
}

Token::Token(std::string title, bool encase)
{
	ID = Utility::NewID();
	if (encase)
	{
		if (title.size() > 0)
		{
			if (title[0] != '<')
				title = '<' + title;
			if (title[title.size() - 1] != '>')
				title = title + '>';
		}
	}
	Title = title;
	Value = "";
	Sibling = NULL;
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
		Sibling = NULL;
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

Token* Token::GetSiblingCopy(Token* token)
{
	if (token)
	{
		std::vector<Token*> siblings;

		Token* sibling = token;

		while (sibling)
		{
			siblings.push_back(new Token(sibling));
			sibling = sibling->Sibling;
		}

		size_t position = siblings.size() - 1;

		while (position > 0)
		{
			siblings[position - 1]->Sibling = siblings[position];
			position--;
		}

		return siblings[0];
	}

	return NULL;
}

std::vector<Token*> Token::GetChildrenCopy(Token* token)
{
	std::vector<Token*> children;
	for (size_t i = 0; i < token->Children.size(); i++)
	{
		children.push_back(new Token(token->Children[i]));
	}
	return children;
}