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
    std::vector<std::vector<Bus>> allBusses;
    std::vector<Bus> line1;
    std::vector<Bus> line2;
    std::vector<Bus> line3;
    explicit Garage(int busId, int busNumber,QGraphicsScene *scene);

    ~Garage() = default;

    void InitGarage();

    void AddBus(Bus bus);

    void DeleteBus();

    Bus* GetBus(int busId, int busLine);

private:


};


#endif //ICP_PROJECT_GARAGE_H
