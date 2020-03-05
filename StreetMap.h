/**
 * @file StreetMap.h
 * @brief StreetMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_STREETMAP_H
#define ICP_PROJECT_STREETMAP_H

#include "Street.h"
#include <iostream>
#include <vector>
#include <map>

#define X    100
#define Y    100

class StreetMap {
public:
    /**
     * @brief initial setup of map
     */
    StreetMap();

    ~StreetMap();

    /**
     * @brief add street on map
     * @param s street
     * @return true if street on coordinates does not exists, otherwise false
     */
    bool AddStreet(Street *s);

    /**
     * @brief searches the map with name of street
     * @param name name of street
     * @return street if found, otherwise nullptr
     */
    Street *GetStreet(const std::string& name);

    std::vector<Street*> Map[X][Y]{};   //!< map of streets
};

#endif