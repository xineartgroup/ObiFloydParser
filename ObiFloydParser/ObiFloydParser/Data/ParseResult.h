#pragma once
#include <string>

class ParseResult
{
public:
	int X;
	int Y;
	bool Success;
	ParseResult();
	ParseResult(int x, int y);
};

