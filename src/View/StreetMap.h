/**
 * @file StreetMap.h
 * @brief StreetMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STREETMAP_H
#define STREETMAP_H

#include "../Bus/BusRouteMap.h"
#include <memory>

class StreetMap : public QGraphicsRectItem
{
public:
    struct stopData {
        Stop *stop;                                 //!< bus stop
        QGraphicsPixmapItem *photo;                 //!< photo of bus stop
        Coordinates::Coordinates_S coordinates;     //!< coordinates of bus stop
    };
    
    /**
     * @brief initial setup of map
     */
    explicit StreetMap(QGraphicsRectItem *parent = nullptr);

    ~StreetMap();

    /**
     * @brief add street on map
     * @param s street
     * @return true if street on coordinates does not exists, otherwise false
     */
    bool AddStreet(std::shared_ptr<Street> s);

    /**
     * @brief add streets on map from file
     * @param pathToFile
     */
     void AddStreets(const std::string& pathToFile);

    /**
     * @brief adds bus stops on street
     * @param pathToFile path to file
     * @param scene graphics scene
     */
    void AddStops(const std::string& pathToFile, QGraphicsScene *scene);

    /**
     * @brief searches the map with name of street
     * @param name name of street
     * @return street if found, otherwise nullptr
     */
    std::shared_ptr<Street> GetStreet(const std::string& name);

    /**
     * @brief update slowdown on street
     * @param name name of street
     * @param updateSlowdown ChangedSlowDownValue to change
     */
    void UpdateStreet(const std::string& name, float updateSlowdown);

    /**
     * @brief get coordinates of stop by name
     * @param name name of stop
     * @return coordinates of stop or nullptr
     */
    static Coordinates::Coordinates_S GetStopByName(const std::string& name);

    /**
     * @brief get bus stop by its coordinates
     * @param x x coordinate
     * @param y y coordinate
     * @return bus stop name
     */
    static std::string GetStopByCoordinates(int x, int y);

    std::vector<std::shared_ptr<Street>> Map[X][Y]{};         //!< map of streets
    static std::vector<stopData> stopList;                    //!< list of stops
    void UpdateAllStreet();
};

#endif // STREETMAP_H
