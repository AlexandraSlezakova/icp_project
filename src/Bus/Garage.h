/**
 * @file Garage.h
 * @brief Garage header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_GARAGE_H
#define ICP_PROJECT_GARAGE_H

#include "Bus.h"
#include "../View/StreetMap.h"
class Garage
{
public:
    std::vector<Bus*> allBuses;    //!< all buses storage

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
    void AddBus(int busId, int busNumber, QGraphicsScene *scene);

    /**
    * @brief move all buses on the scene
    * @param streetMap for CheckSlowDown function
    */
    void MoveAllBuses(StreetMap *streetMap);

    /**
    * @brief get bus from bus storage
    * @param busId id of bus
    * @return bus found bus | nullptr
    */
    Bus* GetBus(int busId);

    /**
     * @brief get bus by photo
     * @param photo photo of bus
     * @return found bus | nullptr
     */
    Bus *GetBusByPhoto(QGraphicsItem *photo);

    /**
    * @brief check street slow down for bus
    * @param streetMap find street in map with slowdown
    * @param bus a bus for which a slowdown applies
    * @return bus with updated data
    */
    Bus* CheckSlowDown(StreetMap *streetMap, Bus *bus);


private:


};


#endif //ICP_PROJECT_GARAGE_H
