/**
 * @file Stop.h
 * @brief Stop header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_STOP_H
#define ICP_PROJECT_STOP_H

#include "Coordinates.h"

class Stop
{
public:
    /**
     * @brief initial setup of bus stop
     * @param coordinates coordinates of bus stop
     */
    Stop(Coordinates *coordinates);

    ~Stop();

    Coordinates *coordinates;   //!< coordinates of bus stop
};


#endif //ICP_PROJECT_STOP_H
