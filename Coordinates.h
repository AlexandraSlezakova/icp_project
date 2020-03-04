#ifndef ICP_PROJECT_COORDINATES_H
#define ICP_PROJECT_COORDINATES_H

class Coordinates
{
public:
    int x{}, y{};

    Coordinates(int x1, int y1);

    ~Coordinates();

    int GetX();

    int GetY();
};
#endif