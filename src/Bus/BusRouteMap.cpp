#include "BusRouteMap.h"

void
BusRouteMap::DrawLine(std::vector<Coordinates::BusStop_S> stopInformation, QString color)
{
    Coordinates *currentCoordinate, *nextCoordinate;
    Square *square;
    QString squareColor, previousColor, originalColor = color;
    int from, to, allSame, iterations, value;
    int x, y, xValue, yValue, yAxis;

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        currentCoordinate = stopInformation[i].coordinates;
        nextCoordinate = stopInformation[i + 1].coordinates;
        IF(originalColor != color, color = originalColor)
        iterations = allSame = 0;
        yAxis = 0;

        if (nextCoordinate->x == currentCoordinate->x) {
            from = currentCoordinate->y;
            to = nextCoordinate->y;
            yAxis = 1;
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

        if (yAxis) {
            xValue = currentCoordinate->x;
        }
        else {
            yValue = currentCoordinate->y;
        }

        for (int index = from; index < to; index++) {
            if (yAxis) {
                yValue = index;
            }
            else {
                xValue = index;
            }

            square = Square::layout[xValue][yValue];
            if (square->hasStop)
                continue;

            squareColor = square->GetColor();

            if (color == "#c0c0c0" && !allSame) {
                if (!previousColor.isEmpty()) {
                    /* difference was found */
                    if (previousColor != squareColor) {
                        color = squareColor;
                        index = from - 1;
                        allSame = 1;
                        iterations = 0;
                    }
                    else {
                        IF(iterations, allSame = 1)
                        iterations++;
                    }
                }
                else {
                    previousColor = squareColor;
                }
                continue;
            }

            if (squareColor == "#c0c0c0" || (index % 2) || color == "#c0c0c0" || color != originalColor) {
                /* color of previous 3 squares wasn't changed, change it now */
                if (allSame && iterations == 2) {
                    for (int j = 1; j < 4; j++) {
                        if (yAxis) {
                            x = xValue;
                            y = index - j;
                        }
                        else {
                            y = yValue;
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