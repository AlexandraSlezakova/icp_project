/**
 * @file StreetMap.h
 * @brief StreetMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREETMAP_H
#define STREETMAP_H

#include "Street.h"
#include "Qt/Square.h"

#define X    138
#define Y    79

class StreetMap : public QGraphicsRectItem
{
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
     * @brief add streets on map from file
     * @param pathToFile
     */
     void AddStreets(const std::string& pathToFile);

    /**
    * @brief adds bus stops on street
    * @param pathToFile
    */
    void AddStops(const std::string& pathToFile);

    /**
     * @brief searches the map with name of street
     * @param name name of street
     * @return street if found, otherwise nullptr
     */
    Street *GetStreet(const std::string& name);


    std::vector<Street*> Map[X][Y]{};   //!< map of streets
    Square *layout[X][Y];
};

#endif // STREETMAP_H
