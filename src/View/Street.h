/**
 * @file Street.h
 * @brief Street header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREET_H
#define STREET_H

#include <iostream>
#include <list>
#include "../Bus/Stop.h"
#include "../View/Square.h"

#define SQUARE_SIZE     20
class Street
{
public:

    /**
     * @brief initial setup of street
     * @param s name of street
     * @param start coordinates of start of street
     * @param end coordinates of end of street
     */
    Street(std::string s, Coordinates::Coordinates_S start, Coordinates::Coordinates_S end,float slowdown);

    ~Street();

    std::string name;                       //!< name of street
    Coordinates::Coordinates_S start;       //!< start of street
    Coordinates::Coordinates_S end;         //!< end of street
    float slowdown;                         //!< slowdown on street
    float previousSlowdown;                 //!< previous slowdown on street
};

#endif // STREET_H
