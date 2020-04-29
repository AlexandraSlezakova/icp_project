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
    Coordinates() = default;

    ~Coordinates() = default;

    typedef struct coordinates {
        int x = 0;                      //!< x coordinate
        int y = 0;                      //!< y coordinate
    } Coordinates_S;

    typedef struct busStop {
        Coordinates_S coordinates;      //!< coordinates
        int stopHour = 0;               //!< hour when the bus is at a stop
        int stopMin = 0;                //!< minute when the bus is at a stop
        std::string name;               //!< name of bus stop
        int id = 0;                     //!< id of bus stop
    } BusStop_S;
};

#endif // COORDINATES_H
