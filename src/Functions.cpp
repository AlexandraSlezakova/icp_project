#include "Functions.h"

Functions::Functions() =default;

std::vector<std::string>
Functions::Split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos;
    std::string token;
    int name_size;

    do
    {
        pos = str.find(delimiter, prev);
        IF(pos == std::string::npos, pos = str.length())
        token = str.substr(prev, pos-prev);

        name_size = token.size();
        if (token[name_size - 1] == '\r' || token[name_size - 1] == '\n') {
            token.erase(name_size - 1);
        }

        IF(!token.empty(), tokens.push_back(token))
        prev = pos + delimiter.length();

    } while (pos < str.length() && prev < str.length());

    return tokens;
}

std::string
Functions::GetAbsolutePath(const char *fileName)
{
    std::string path  = realpath(fileName, nullptr);
    IF (path.empty(), std::cerr << "Error: Couldn't open file " << fileName; exit(1))
    return path;
}
