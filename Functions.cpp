//
// Created by alexandra on 5. 3. 2020.
//

#include "Functions.h"

Functions::Functions() =default;

std::vector<std::string>
Functions::Split(const std::string& str)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    std::string token, delimiter = " ";

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
