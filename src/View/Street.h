/**
 * @file Street.h
 * @brief Street header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREET_H
#define STREET_H

#include <iostream>
#include <list>
#include "../Bus/Bus.h"

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
    Street(std::string s, Coordinates *start, Coordinates *end);

    ~Street();

    /**
     * @brief set stop on street
     * @param stop
     */
    void SetStop(Stop* stop);

    std::string name;                       //!< name of street
    Coordinates* start;                     //!< start of street
    Coordinates* end;                       //!< end of street

protected:
    std::vector<Stop*> busStopsList;            //!< bus stops list
};

#endif // STREET_H
