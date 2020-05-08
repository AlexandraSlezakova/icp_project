/**
 * @file Garage.h
 * @brief Garage header file
 * @author Martin Vadura, Alexandra Slezakova
 */

#ifndef ICP_PROJECT_GARAGE_H
#define ICP_PROJECT_GARAGE_H

#include "Bus.h"

class Garage
{
public:
    std::vector<Bus*> allBuses;             //!< bus storage
    std::map<int, int> routeTime;           //!< route length in minutes
    int deletedBus;
    /**
    * @brief garage constructor
    */
    Garage();

    ~Garage() = default;

    /**
     * @brief add bus to garage
     * @param id id of bus
     * @param busNumber number of bus
     * @param scene graphics scene
     */
    void AddBus(int id, int busNumber, QGraphicsScene *scene, int busCount);

    /**
     * @brief move all buses on the scene
     * @param streetMap for CheckRoad function
     * @param scene graphics scene
     */
    void MoveAllBuses(StreetMap *streetMap, QGraphicsScene *scene);

    /**
     * @brief get bus by photo
     * @param photo photo of bus
     * @return found bus | nullptr
     */
    Bus *GetBusByPhoto(QGraphicsItem *photo);

    /**
    * @brief check street slow down for bus and road stop
    * @param streetMap find street in map with slowdown
    * @param bus a bus for which a slowdown applies
    * @return bus with updated data
    */
    Bus* CheckRoad(StreetMap *streetMap, Bus *bus);

    /**
     * @brief delete bus from bus storage and scene
     * @param bus bus to delete
     * @param scene graphics scene
     */
    void DeleteBus(Bus *bus, QGraphicsScene *scene);

    /**
    * @brief check roadblock on long distance, on all bus road
    * @param bus bus to check
    */
    bool CheckRoadBlockLongDistance(Bus *bus);

    /**
     * @brief delete all buses from graphics scene
     * @param scene graphics scene
     */
    void DeleteBuses(QGraphicsScene *scene);
private:

    /**
    * @brief check roadblock on short distance, on first next stop
    * and route between current and next busstop
    * @param bus bus to check
    */
    bool CheckRoadBlockShortDistance(Bus *bus);
};


#endif //ICP_PROJECT_GARAGE_H
