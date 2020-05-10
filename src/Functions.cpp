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

std::vector<char*> Functions::pathPointers;

std::string
Functions::GetAbsolutePath(const char *fileName)
{
    char *path = realpath(fileName, nullptr);
    IF (path == nullptr, std::cerr << "Error: Couldn't open file " << fileName; exit(1))
    pathPointers.push_back(path);
    std::string string = path;
    return string;
}
