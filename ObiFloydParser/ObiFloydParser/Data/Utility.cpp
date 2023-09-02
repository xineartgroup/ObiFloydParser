#include "Utility.h"

unsigned long Utility::LastID = 0;

unsigned long Utility::NewID()
{
    LastID++;
    return LastID;
}

std::vector<std::string> Utility::Split(std::string s, std::string delimiter)
{
    size_t pos_start = 0;
    size_t pos_end = 0;
    size_t delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string Utility::Replace(std::string str, const std::string from, const std::string to)
{
    bool found = true;

    size_t start_pos = 0;

    while (found)
    {
        start_pos = str.find(from, start_pos);

        if (start_pos == std::string::npos)
        {
            found = false;
        }
        else
        {
            str.replace(start_pos, from.length(), to);
        }

        start_pos = start_pos + from.length();
    }

    return str;
}