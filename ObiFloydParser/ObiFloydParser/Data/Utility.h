#pragma once

#include <vector>
#include <string>

class Utility
{
public:
	static unsigned long LastID;

	static unsigned long NewID();
	static std::vector<std::string> Split(std::string, std::string);
	static std::string Replace(std::string, const std::string, const std::string);
};

