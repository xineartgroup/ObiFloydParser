#include "ParseResult.h"

ParseResult::ParseResult()
{
	X = 0;
	Y = 0;
	Success = false;
}

ParseResult::ParseResult(int x, int y)
{
	X = x;
	Y = y;
	Success = false;
}
