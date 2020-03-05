#include "Functions.h"

Functions::Functions() =default;

std::vector<std::string>
Functions::Split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    std::string token;

    do
    {
        pos = str.find(delimiter, prev);
        IF(pos == std::string::npos, pos = str.length())
        token = str.substr(prev, pos-prev);
        IF(!token.empty(), tokens.push_back(token))
        prev = pos + delimiter.length();

    } while (pos < str.length() && prev < str.length());

    return tokens;
}
