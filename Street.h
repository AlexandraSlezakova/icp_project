/**
 * @file Street.h
 * @brief Street header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_STREET_H
#define ICP_PROJECT_STREET_H

#include <iostream>
#include <utility>
#include <list>
#include "Stop.h"

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
    void SetStop(Stop *stop);

    /**
     * @brief get list of bus stops on street
     * @return list of bus stops
     */
    std::list<Stop*> GetBusStopsList();

    /**
     * @brief get list of coordinates of street
     * @return list of coordinates
     */
    std::list<Coordinates*> GetCoordinatesList();

    std::string name;   //!< name of street
    Coordinates *start; //!< start of street
    Coordinates *end;   //!< end of street

protected:
    std::list<Stop*> busStopsList;              //!< bus stops list
    std::list<Coordinates*> coordinatesList;    //!< list of coordinates of street

};
#endif //ICP_PROJECT_STREET_H
