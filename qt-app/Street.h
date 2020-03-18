/**
 * @file Street.h
 * @brief Street header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREET_H
#define STREET_H

#include <iostream>
#include <list>
#include "Stop.h"

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
    void SetStop(Stop *stop);

    /**
     * @brief get list of bus stops on street
     * @return list of bus stops
     */
    std::vector<Stop*> GetBusStopsList();

    /**
     * @brief get list of coordinates of street
     * @return list of coordinates
     */
    std::list<Coordinates*> GetCoordinatesList();

    /**
     * @brief adds image of bus stop on square
     * where bus stops of each street are
     * @param scene scene where to put image
     */
    void AddStopsToScene(QGraphicsScene *scene);

    std::string name;                       //!< name of street
    Coordinates *start;                     //!< start of street
    Coordinates *end;                       //!< end of street

protected:
    std::list<Coordinates*> coordinatesList;    //!< list of coordinates of street
    std::vector<Stop*> busStopsList;            //!< bus stops list
};

#endif // STREET_H
