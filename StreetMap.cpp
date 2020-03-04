#include "StreetMap.h"

StreetMap::StreetMap()
{
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            Map[x][y] = new Coordinates(x, y);
        }
    }
}


StreetMap::~StreetMap()
{
    for (auto &x : Map) {
        for (auto &y : x) {
           delete y;
        }
    }
}