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

    /**
    * @brief garage constructor
    */
    Garage();

    ~Garage() = default;

    /**
     * @brief add bus to garage
     * @param id id of bus
     * @param busNumber number of bus
     * @param scene
     */
    void AddBus(int id, int busNumber, QGraphicsScene *scene, int iteration = 0);

    /**
     * @brief move all buses on the scene
     * @param streetMap for CheckRoad function
     * @param scene
     */
    void MoveAllBuses(StreetMap *streetMap, QGraphicsScene *scene);

    /**
    * @brief get bus from bus storage
    * @param id id of bus
    * @return bus found bus | nullptr
    */
    Bus* GetBus(int id);

    /**
     * @brief get bus by photo
     * @param photo photo of bus
     * @return found bus | nullptr
     */
    Bus *GetBusByPhoto(QGraphicsItem *photo);

    /**
    * @brief check street slow down for bus and roadstop
    * @param streetMap find street in map with slowdown
    * @param bus a bus for which a slowdown applies or stop due to stoproad
    * @return bus with updated data
    */
    Bus* CheckRoad(StreetMap *streetMap, Bus *bus);

    /**
     * @brief delete bus from bus storage and scene
     * @param bus bus to delete
     * @param scene
     */
    void DeleteBus(Bus *bus, QGraphicsScene *scene);

    bool CheckRoadBlockLongDistace(Bus *bus);

    void DeleteBuses(QGraphicsScene *scene);
private:
    bool CheckRoadBlockShortDistace(Bus *bus);
};


#endif //ICP_PROJECT_GARAGE_H
