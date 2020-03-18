#include "BusRouteMap.h"

void
BusRouteMap::paintEvent(QPaintEvent *event)
{

}

void
BusRouteMap::DrawLine(Square *layout[X][Y])
{
    auto *bus = new Bus(1);
    std::vector<Coordinates*> coordinates = bus->stopCoordinates;
    // TODO
}

