#include "BusRouteMap.h"

void
BusRouteMap::DrawLine(std::vector<Coordinates::BusStop_S> stopInformation, QString color)
{
    Coordinates *currentCoordinate, *nextCoordinate;
    Square *square;
    QString square_color, previous_color, original_color = color;
    int from, to, all_same, iterations, value;
    int x, y, x_value, y_value, y_axis;

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        currentCoordinate = stopInformation[i].coordinates;
        nextCoordinate = stopInformation[i + 1].coordinates;
        IF(original_color != color, color = original_color)
        iterations = all_same = 0;
        y_axis = 0;

        if (nextCoordinate->x == currentCoordinate->x) {
            from = currentCoordinate->y;
            to = nextCoordinate->y;
            y_axis = 1;
        }
        else {
            from = currentCoordinate->x;
            to = nextCoordinate->x;
        }

        /* swap coordinates */
        if (from > to) {
            value = from;
            from = to;
            to = value;
        }

        if (y_axis) {
            x_value = currentCoordinate->x;
        }
        else {
            y_value = currentCoordinate->y;
        }

        for (int index = from; index < to; index++) {
            if (y_axis) {
                y_value = index;
            }
            else {
                x_value = index;
            }

            square = Square::layout[x_value][y_value];
            if (square->hasStop)
                continue;

            square_color = square->GetColor();

            if (color == "#c0c0c0" && !all_same) {
                if (!previous_color.isEmpty()) {
                    /* difference was found */
                    if (previous_color != square_color) {
                        color = square_color;
                        index = from - 1;
                        all_same = 1;
                        iterations = 0;
                    }
                    else {
                        IF(iterations, all_same = 1)
                        iterations++;
                    }
                }
                else {
                    previous_color = square_color;
                }
                continue;
            }

            if (square_color == "#c0c0c0" || (index % 2) || color == "#c0c0c0" || color != original_color) {
                /* color of previous 3 squares wasn't changed, change it now */
                if (all_same && iterations == 2) {
                    for (int j = 1; j < 4; j++) {
                        if (y_axis) {
                            x = x_value;
                            y = index - j;
                        }
                        else {
                            y = y_value;
                            x = index - j;
                        }
                        Square::layout[x][y]->SetColor(color);
                        iterations = 0;
                    }
                }
                square->SetColor(color);
            }
        }
    }
}