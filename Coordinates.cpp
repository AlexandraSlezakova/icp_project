#include "Coordinates.h"

Coordinates::Coordinates(int x1, int y1)
{
    x = x1;
    y = y1;
}

Coordinates::~Coordinates()= default;

int
Coordinates::GetX()
{
    return x;
}

int
Coordinates::GetY()
{
    return y;
}