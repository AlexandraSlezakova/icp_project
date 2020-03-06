#include "Street.h"

Street::Street(std::string s, Coordinates *start, Coordinates *end)
{
    this->name = std::move(s);
    this->start = start;
    this->end = end;

    for (int x = start->x; x < end->x; x++) {
        for (int y = start->y; y < end->y; y++) {
            coordinatesList.push_back(new Coordinates(x, y));
        }
    }
}

Street::~Street() =default;

void
Street::SetStop(Stop *stop)
{
    busStopsList.push_back(stop);
}

std::list<Stop*>
Street::GetBusStopsList()
{
    return busStopsList;
}

std::list<Coordinates*>
Street::GetCoordinatesList()
{
    return coordinatesList;
}
