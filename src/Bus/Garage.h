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
    std::vector<std::vector<Bus>> allBuses;
    std::vector<Bus*> line1;
    std::vector<Bus> line2;
    std::vector<Bus> line3;
    explicit Garage(int busId, int busNumber,QGraphicsScene *scene);

    ~Garage() = default;

    void AddBus(Bus bus);

    void MoveAllBuses(StreetMap *streetMap);

    Bus* GetBus(int busId, int busLine);

    Bus* CheckSlowDown(StreetMap *streetMap, Bus *bus);


private:


};


#endif //ICP_PROJECT_GARAGE_H
