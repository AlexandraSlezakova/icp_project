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

std::vector<Stop*>
Street::GetBusStopsList()
{
    return busStopsList;
}

std::list<Coordinates*>
Street::GetCoordinatesList()
{
    return coordinatesList;
}

void
Street::AddStopsToScene(QGraphicsScene *scene)
{
    for (const auto& i : busStopsList) {
        auto *item = new QGraphicsPixmapItem(QPixmap("/home/alexandra/Projects/icp_project/images/bus_stop.jpeg"));
        scene->addItem(item);
        item->setScale(0.05);
        /* put image in the center of square */
        item->setPos(i->coordinates->x * SQUARE_SIZE + 2, i->coordinates->y * SQUARE_SIZE + 1);
    }
}
