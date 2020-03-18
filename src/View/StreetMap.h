/**
 * @file StreetMap.h
 * @brief StreetMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREETMAP_H
#define STREETMAP_H

#include "../Bus/BusRouteMap.h"

class StreetMap : public QGraphicsRectItem
{
public:
    /**
     * @brief initial setup of map
     */
    explicit StreetMap(QGraphicsRectItem *parent = nullptr);

    ~StreetMap();

    /**
     * @brief add street on map
     * @param s street
     * @param color color of square in hex format
     * @return true if street on coordinates does not exists, otherwise false
     */
    bool AddStreet(Street *s, const QString &color);

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
    Square *layout[X][Y]{};
};

#endif // STREETMAP_H