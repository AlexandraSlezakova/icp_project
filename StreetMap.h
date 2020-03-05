#ifndef ICP_PROJECT_STREETMAP_H
#define ICP_PROJECT_STREETMAP_H

#include "Street.h"
#include <iostream>
#include <vector>
#include <map>

#define X    100
#define Y    100

class StreetMap {
public:
    std::vector<Street*> Map[X][Y]{};

    StreetMap();

    ~StreetMap();

    bool AddStreet(Street *s);

    Street *GetStreet(const std::string& name);
};

#endif