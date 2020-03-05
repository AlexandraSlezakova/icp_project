#ifndef ICP_PROJECT_COORDINATES_H
#define ICP_PROJECT_COORDINATES_H

#include <string>

#define IF(CONDITION, IF_BLOCK) if (CONDITION) {IF_BLOCK;}
#define IF_RETURN(CONDITION, VALUE) if (CONDITION) return VALUE;

class Coordinates
{
public:
    int x{}, y{};

    Coordinates(int x1, int y1);

    ~Coordinates();
};
#endif