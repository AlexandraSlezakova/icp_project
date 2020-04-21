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
    std::vector<Bus*> line1;    //!< array for all bus line 1
    std::vector<Bus*> line2;     //!< array for all bus line 2
    std::vector<Bus*> line3;     //!< array for all bus line 3
    std::vector<Bus*> allbus;     //!< array for all bus

    /**
    * @brief garage constructor
    * @param id id of bus
    * @param busNumber number of bus
    * @param scene
    */
    explicit Garage(int busId, int busNumber,QGraphicsScene *scene);

    ~Garage() = default;

    void AddBus(Bus bus);

    /**
    * @brief move to all buses on scene
    * @param streetMap for CheckSlowDown function
    */
    void MoveAllBuses(StreetMap *streetMap);

    /**
    * @brief return right bus from line array
    * @param busId id of bus
    * @param busLine number of bus
    * @return bus
    */
    Bus* GetBus(int busId, int busLine);

    /**
    * @brief chcek street slow down for bus
    * @param streetMap for get slowdown on street
    * @param bus for which check is in running
    * @return bus to update data
    */
    Bus* CheckSlowDown(StreetMap *streetMap, Bus *bus);


private:

    bool CheckRoadBlock(StreetMap *streetMap, Bus *bus);
};


#endif //ICP_PROJECT_GARAGE_H
