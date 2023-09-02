#include "Tokenizer.h"

Token* Tokenizer::GetToken(std::string line)
{
    std::vector<std::string> strTokens = Utility::Split(line, ":=");

    if (strTokens.size() == 2)
    {
        Token* token = new Token(strTokens[0], true);

        std::vector<std::string> children = Utility::Split(strTokens[1], "|");

        if (strTokens[1][0] == '<')
        {
            for (size_t i = 0; i < children.size(); i++)
            {
                token->Children.push_back(GetChild(children[i]));
            }
        }
        else
        {
            for (size_t i = 0; i < children.size(); i++)
            {
                token->Children.push_back(new Token(children[i], children[i]));
            }
        }

        return token;
    }

    return nullptr;
}

std::string Tokenizer::GetTokenTitle(std::string line)
{
    std::vector<std::string> strTokens = Utility::Split(line, ":=");

    if (strTokens.size() == 2)
    {
        return strTokens[0];
    }

    return "";
}

Token* Tokenizer::GetChild(std::string strChild)
{
    size_t pos = strChild.find('{');
    if (pos >= 0 && pos < strChild.size())
    {
        size_t pos2 = strChild.find('}');
        std::string strChild1 = strChild.substr(0, pos);
        std::string strChild2 = strChild.substr(pos + 1, pos2 - pos - 1);
        Token* token1 = new Token(Utility::Split(strChild1, "><"));
        token1->Repeat = true;
        token1->Alternate = new Token(Utility::Split(strChild2, "><"));
        return token1;
    }
    else
    {
        return new Token(Utility::Split(strChild, "><"));
    }
    return nullptr;
}
