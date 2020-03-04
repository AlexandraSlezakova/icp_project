#include "StreetMap.h"

StreetMap::StreetMap(int maxX1, int maxY1)
{
//    maxX = maxX1;
//    maxY = maxY1;

    for (int x = 0; x < maxX1; x++) {
        for (int y = 0; y < maxY1; y++) {
            Map[x] = new Coordinates(x, y);
        }
    }
}

StreetMap::~StreetMap()
{
//    for (int x = 0; x < maxX; x++) {
//        for (int y = 0; y < maxY; y++) {
//           // delete Map.back();
//        }
//    }
}