/**
 * @file Coordinates.h
 * @brief Coordinates header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef COORDINATES_H
#define COORDINATES_H

#include "../Functions.h"

class Coordinates
{
public:
    int x{}, y{};   //!< x and y coordinate

    /**
     * @brief initial setup of coordinates
     * @param x1 x coordinate
     * @param y1 y coordinate
     */
    Coordinates(int x1, int y1);

    ~Coordinates();
};

#endif // COORDINATES_H
