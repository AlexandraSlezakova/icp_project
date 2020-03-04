#ifndef ICP_PROJECT_STREETMAP_H
#define ICP_PROJECT_STREETMAP_H

#include "Coordinates.h"
#include <iostream>
#include <vector>

class StreetMap {
public:
    static int maxX, maxY;
    Coordinates **Map{};

    StreetMap(int maxX1, int maxY1);

    ~StreetMap();

private:

};

#endif