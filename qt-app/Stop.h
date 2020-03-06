/**
 * @file Stop.h
 * @brief Stop header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STOP_H
#define STOP_H

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

#endif // STOP_H
