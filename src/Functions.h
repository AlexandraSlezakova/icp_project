/**
 * @file Functions.h
 * @brief Functions header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <QDir>

#define IF(CONDITION, IF_BLOCK) if (CONDITION) {IF_BLOCK;}
#define IF_RETURN(CONDITION, VALUE) if (CONDITION) return VALUE;
#define IF_ELSE(CONDITION, IF_BLOCK, ELSE_BLOCK) if (CONDITION) {IF_BLOCK;} else {ELSE_BLOCK;}

class Functions
{
public:
    Functions();

    /**
     * @brief split string
     * @param str string to split
     * @return vector of strings
     */
    static std::vector<std::string> Split(const std::string& str, const std::string& delimiter);

    /**
     * @brief absolute path to file
     * @param fileName name of file
     * @return absolute path as string
     */
    static std::string GetAbsolutePath(const char* fileName);
};

#endif // FUNCTIONS_H
