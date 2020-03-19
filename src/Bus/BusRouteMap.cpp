#include "BusRouteMap.h"

void
BusRouteMap::DrawLine(std::vector<Coordinates::BusStop_S> stopInformation, Square *layout[X][Y], const QString& color)
{
    Coordinates *currentCoordinate, *nextCoordinate;
    int from , to;

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        currentCoordinate = stopInformation[i].coordinates;
        nextCoordinate = stopInformation[i + 1].coordinates;

        /* moving in x axis */
        if (nextCoordinate->x == currentCoordinate->x) {
            from = currentCoordinate->y;
            to = nextCoordinate->y;

            if (currentCoordinate->y > nextCoordinate->y) {
                from = nextCoordinate->y;
                to = currentCoordinate->y;
            }

            for (int y = from; y < to; y++) {
                if (layout[currentCoordinate->x][y]->hasStop)
                    continue;

                layout[currentCoordinate->x][y]->SetColor(color);
            }

        } /* moving in y axis */
        else {
            from = currentCoordinate->x;
            to = nextCoordinate->x;

            if (currentCoordinate->x > nextCoordinate->x) {
                from = nextCoordinate->x;
                to = currentCoordinate->x;
            }

            for (int x = from; x < to; x++) {
                if (layout[x][currentCoordinate->y]->hasStop)
                    continue;

                layout[x][currentCoordinate->y]->SetColor(color);
            }
        }
    }

}

