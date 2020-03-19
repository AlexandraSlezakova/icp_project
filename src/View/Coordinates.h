/**
 * @file Coordinates.h
 * @brief Coordinates header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef COORDINATES_H
#define COORDINATES_H

#include "../Functions.h"
#include "Timer.h"

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

    typedef struct busStop {
        Coordinates *coordinates{};   //!< coordinates of bus stop
        int stopHour = 0;
        int stopMin = 0;
    } BusStop_S;
};

#endif // COORDINATES_H
