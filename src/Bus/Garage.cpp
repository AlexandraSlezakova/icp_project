#include "Garage.h"

Garage::Garage(int busId, int busNumber,QGraphicsScene *scene)
{
    Bus *bus = new Bus(busId,busNumber);
    bus->InitBus(scene);
    line1.push_back(*bus);
    allBusses.push_back(line1);
}

void Garage::AddBus(Bus bus) {

}

Bus* Garage::GetBus(int busId, int busLine) {
    if (busLine == 1)
    {
        for ( int i = 0; i <= line1.size(); i++ )
            if (busId == line1[i].id_)
                return &line1[i];
    }
    //return &line1[0];
}
