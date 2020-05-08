#include "BusRouteMap.h"

void
BusRouteMap::DrawLine(std::vector<Coordinates::BusStop_S> stopInformation, QString color)
{
    Coordinates::Coordinates_S currentCoordinate, nextCoordinate;
    int from , to;

    for (int i = 0; i < (int)stopInformation.size() - 1; i++) {
        currentCoordinate = stopInformation[i].coordinates;
        nextCoordinate = stopInformation[i + 1].coordinates;

        /* moving along the x axis */
        if (nextCoordinate.x == currentCoordinate.x) {
            from = currentCoordinate.y;
            to = nextCoordinate.y;

            if (currentCoordinate.y > nextCoordinate.y) {
                from = nextCoordinate.y;
                to = currentCoordinate.y;
            }

            for (int y = from; y < to; y++) {
                if (Square::layout[currentCoordinate.x][y]->hasStop)
                    continue;

                if (Square::layout[currentCoordinate.x][y]->GetColor() != "#ff0000")
                    Square::layout[currentCoordinate.x][y]->SetColor(color);
            }

        } /* moving along the y axis */
        else {
            from = currentCoordinate.x;
            to = nextCoordinate.x;

            if (currentCoordinate.x > nextCoordinate.x) {
                from = nextCoordinate.x;
                to = currentCoordinate.x;
            }

            for (int x = from; x < to; x++) {
                if (Square::layout[x][currentCoordinate.y]->hasStop)
                    continue;

                if (Square::layout[x][currentCoordinate.y]->GetColor() != "#ff0000")
                    Square::layout[x][currentCoordinate.y]->SetColor(color);
            }
        }
    }

}

