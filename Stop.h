#ifndef ICP_PROJECT_STOP_H
#define ICP_PROJECT_STOP_H

#include "Coordinates.h"

class Stop
{
public:
    Stop(Coordinates *coordinates);

    ~Stop();

    Coordinates *coordinates;
};


#endif //ICP_PROJECT_STOP_H
