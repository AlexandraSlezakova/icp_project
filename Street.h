#ifndef ICP_PROJECT_STREET_H
#define ICP_PROJECT_STREET_H

#include <iostream>
#include <utility>
#include <list>
#include "Stop.h"

class Street
{
public:
    Street(std::string s, Coordinates *start, Coordinates *end);

    ~Street();

    void SetStop(Stop *stop);

    std::list<Stop*> GetBusStopsList();

    std::list<Coordinates*> GetCoordinatesList();

    std::string name;

    Coordinates *start;

    Coordinates *end;

private:

    std::list<Stop*> busStopsList;

    std::list<Coordinates*> coordinatesList;

};
#endif //ICP_PROJECT_STREET_H
