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
     * @param stopName bus stop name
     * @param coordinates coordinates of bus stop
     */
    explicit Stop(std::string stopName, Coordinates *coordinates);

    ~Stop();

    Coordinates *coordinates;   //!< coordinates of bus stop
    std::string stopName;       //!< bus stop name
};


#endif //ICP_PROJECT_STOP_H
