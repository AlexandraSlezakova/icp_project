#ifndef ICP_PROJECT_STREETMAP_H
#define ICP_PROJECT_STREETMAP_H

#include "Coordinates.h"
#include <iostream>
#include <vector>

#define X    100
#define Y    100

class StreetMap {
public:
    Coordinates *Map[X][Y]{};

    StreetMap();

    ~StreetMap();
};

#endif