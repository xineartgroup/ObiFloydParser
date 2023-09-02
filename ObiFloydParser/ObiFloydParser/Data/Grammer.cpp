#include "Grammer.h"

Token* Grammer::head = nullptr;
size_t  Grammer::furthest = 0;
std::string Grammer::expected;
std::vector<std::string> Grammer::lines;

void Grammer::ReadGrammer(std::string text)
{
    std::istringstream f(text);
    std::string line;

    while (std::getline(f, line))
    {
        if (line != "")
        {
            lines.push_back(line);
        }
    }
}

ParseResult Grammer::ParseText(std::string text)
{
    furthest = 0;
    Cleanup();
    if (lines.size() > 0)
    {
        head = Tokenizer::GetToken(lines[0]);
        return ParseText(text, head, nullptr, ParseResult());
    }
    return ParseResult();
}

std::string Grammer::GetParseString(std::string text, bool verbose)
{
    std::string result = "";

    ParseResult parseResult = ParseText(text);

    if (verbose)
    {
        result = "\n" + GetParseStructure() + "\n";
    }

    result = result + text + "\n";

    if (parseResult.Success && parseResult.X == text.size())
    {
        result = result + "Success!!!";
    }
    else
    {
        for (size_t i = 0; i <= text.size(); i++)
        {
            result = (i == furthest) ? result + "^" : result + " ";
        }

        result = result + "\n" + expected + " expected.";
    }

    return result;
}

void Grammer::Cleanup()
{
    if (head)
    {
        head->Dispose();
        delete head;
    }
}

std::string Grammer::GetParseStructure()
{
    return GetParseStructure(head, 0);
}

std::string Grammer::GetParseStructure(Token* token, int depth)
{
    std::string text = "";

    if (token)
    {
        std::string space = "";
        for (int j = 0; j < depth; j++)
        {
            space = space + "    ";
        }

        text = space + token->Title + "\n";

        if (token->Selection >= 0 && token->Selection < (int)token->Children.size())
        {
            std::vector<Token*> siblings = token->Children[token->Selection]->GetSiblings();
            for (Token* sibling : siblings)
            {
                text = text + GetParseStructure(sibling, depth + 1);
            }
            text = text + space + Utility::Replace(token->Title, "<", "</") + "\n";
        }
    }

    return text;
}

ParseResult Grammer::ParseText(std::string text, Token* token, Token* next, ParseResult pointer)
{
    ParseResult result;

    result.Success = false;
    result.X = pointer.X;

    if (pointer.X < text.size())
    {
        if (token->Children.size() == 0 && token->Value != "")
        {
            if (text.substr(result.X, token->Value.size()) == token->Value)
            {
                result.Success = true;
                result.X = result.X + (int)token->Value.size();
            }
        }
        else
        {
            for (size_t i = 0; i < token->Children.size(); i++)
            {
                if (result.X < text.size())
                {
                    ParseResult temp = pointer;
                    temp.Success = true;

                    SetSiblingsChildren(token->Children[i]);

                    std::vector<Token*> siblings = token->Children[i]->GetSiblings();

                    Token* current = nullptr;

                    for (size_t j = 0;j < siblings.size(); j++)
                    {
                        current = siblings[j];
                        if (temp.Success)
                        {
                            temp = ParseText(text, siblings[j], (j + 1 < siblings.size()) ? siblings[j + 1] : nullptr, temp);
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (temp.Success)
                    {
                        result = temp;
                        token->Selection = (int)i;
                        if (furthest <= result.X)
                        {
                            if (result.X == text.size() || token->Children[i]->Children.size() == 0)
                            {
                                expected = "[SUCCESS]";
                                furthest = result.X;
                                return result;
                            }
                            else
                            {
                                expected = "Not";
                            }
                        }
                        if (result.X < text.size())
                        {
                            if (token->Children[i]->Repeat)
                            {
                                AddRepeatSibling(token, i);
                            }
                        }
                    }
                    else
                    {
                        if (expected == "[SUCCESS]")
                        {
                            if (next)
                            {
                                if (next->Children.size() == 1)
                                    expected = next->Children[0]->Title;
                                else
                                    expected = next->Title;
                            }
                            else
                            {
                                if (current && current->Children.size() == 1)
                                    expected = current->Children[0]->Title;
                                else
                                    expected = token->Title;
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

void Grammer::SetSiblingsChildren(Token* token)
{
    std::vector<Token*> siblings = token->GetSiblings();

    for (size_t index = 0; index < lines.size(); index++)
    {
        for (size_t j = 0; j < siblings.size(); j++)
        {
            if (siblings[j]->Children.size() == 0)
            {
                if (siblings[j]->Title == Tokenizer::GetTokenTitle(lines[index]))
                {
                    Token* child = Tokenizer::GetToken(lines[index]);

                    if (child)
                    {
                        for (size_t i = 0; i < child->Children.size(); i++)
                        {
                            siblings[j]->Children.push_back(child->Children[i]);
                        }

                        delete child;
                    }
                }
            }
        }
    }
}

void Grammer::AddRepeatSibling(Token* token, size_t index)
{
    if (token->Children.size() > index)
    {
        Token* child = token->Children[index];

        std::vector<Token*> siblings = token->GetSiblings();

        for (size_t index = 0; index < lines.size(); index++)
        {
            for (size_t j = 0; j < siblings.size(); j++)
            {
                if (siblings[j]->Title == Tokenizer::GetTokenTitle(lines[index]))
                {
                    Token* parent = Tokenizer::GetToken(lines[index]);

                    if (parent)
                    {
                        for (Token* temp : parent->Children)
                        {
                            if (temp->Title == child->Title)
                            {
                                Token* sibling = child->Sibling;

                                siblings.clear();
                                siblings.push_back(temp);

                                while (sibling)
                                {
                                    siblings.push_back(new Token(sibling->Title, sibling->Value));
                                    sibling = sibling->Sibling;
                                }

                                sibling = temp->Alternate;

                                while (sibling)
                                {
                                    siblings.push_back(new Token(sibling->Title, sibling->Value));
                                    sibling = sibling->Sibling;
                                }

                                size_t position = siblings.size() - 1;

                                while (position > 0)
                                {
                                    siblings[position - 1]->Sibling = siblings[position];
                                    position--;
                                }

                                token->Children.push_back(temp);

                                delete parent;

                                return;
                            }
                        }

                        delete parent;
                    }
                }
            }
        }
    }
}