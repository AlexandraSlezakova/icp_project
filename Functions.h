/**
 * @file Functions.h
 * @brief Functions header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_FUNCTIONS_H
#define ICP_PROJECT_FUNCTIONS_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#define IF(CONDITION, IF_BLOCK) if (CONDITION) {IF_BLOCK;}
#define IF_RETURN(CONDITION, VALUE) if (CONDITION) return VALUE;

class Functions {
public:
    Functions();

    /**
     * @brief split string
     * @param str string to split
     * @return vector of strings
     */
    static std::vector<std::string> Split(const std::string& str);

};


#endif
