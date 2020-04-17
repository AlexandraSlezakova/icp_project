
#include "Garage.h"

Garage::Garage(int busId, int busNumber,QGraphicsScene *scene)
{
    Coordinates *pos = new Coordinates(0,0);
    Bus *bus = new Bus(busId, busNumber, pos);
    bus->InitBus(scene);
    bus->MoveBus();
    line1.push_back(bus);
    //allBusses.push_back(line1);
}

void
Garage::AddBus(Bus bus)
{

}

Bus* Garage::GetBus(int busId, int busLine) {
    if (busLine == 1) {
        for (auto & i : line1) {
            if (busId == i->id_)
                return i;
        }
    }
    return nullptr;
}

void Garage::MoveAllBuses(StreetMap *streetMap) {
    for (int i = 0; i < line1.size(); i++ ) {
        line1[i] = CheckSlowDown(streetMap,line1[i]);
        line1[i]->MoveBus();
    }

    for(auto bus : line2)
    {
        //this->CheckSlowDown(streetMap,&bus);
        bus.MoveBus();
    }
    for(auto bus : line3)
    {
        //this->CheckSlowDown(streetMap,&bus);
        bus.MoveBus();
    }
}

Bus* Garage::CheckSlowDown(StreetMap *streetMap, Bus *bus) {

    std::vector<std::string> currentSplit;
    std::vector<std::string> nextSplit;
    std::string streetName = "a";
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int secNow = Timer::GetSecond();
    Street *street;
    int timeAdd;
    int i = 0;
    int stopTime;
    int pop;

    int nxt, nw, mn;
    for (; i < bus->stopInformation.size() - 2; i++) {
        nxt = bus->stopInformation[i+1].stopHour * 60 + bus->stopInformation[i+1].stopMin;
        nw  = hourNow * 60 + minuteNow;
        mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

        if (nxt > nw and nw >= mn)
            break;
    }

    if (bus->stopInformation[i].name.empty())
        return bus;

    currentSplit = Functions::Split(bus->stopInformation[i].name, "-");
    nextSplit = Functions::Split(bus->stopInformation[i+1].name, "-");
    /* get name of street where bus is
     * example: Pešinova-Škálova and Pešinova-Karlova
     * - bus is at Pešinova street */
    streetName = currentSplit[0] == nextSplit[0] or currentSplit[0] == nextSplit[1]
            ? currentSplit[0]
            : currentSplit[1];

    if (streetName == "a")
        std::cerr << "Error: Street name wasn't found" << std::endl;

    street = streetMap->GetStreet(streetName);

    if ((bus->stopInformation[i].name != bus->currentBusStop.name and bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        or street->pastslowdown != street->slowdown) {

        if (bus->stopInformation[i].name != bus->currentBusStop.name
            && bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        {
            bus->currentBusStop = bus->stopInformation[i];
            bus->nextBusStop = bus->stopInformation[i + 1];
        }
        else {
             bus->currentBusStop.coordinates = bus->busPosition;
             bus->currentBusStop.stopHour = hourNow;
             bus->currentBusStop.stopMin = minuteNow;
        }

        if (minuteNow < bus->currentBusStop.stopMin)
            timeAdd = 60 + minuteNow - bus->currentBusStop.stopMin;
        else
            timeAdd = minuteNow - bus->currentBusStop.stopMin;

        if (bus->nextBusStop.stopMin < bus->currentBusStop.stopMin)
            stopTime = 60 +  bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;
        else
            stopTime = bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;

        int k =  stopTime;
        stopTime = round(stopTime * street->slowdown);
        k = stopTime - k;

        if(bus->currentBusStop.stopMin + stopTime > 60) {
            bus->stopInformation[i+1].stopHour += 1;
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime - 60;
        }
        else {
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime;
        }

        i++;
        for (; i < bus->stopInformation.size() - 2; i++) {
            if (bus->stopInformation[i].coordinates->x + bus->stopInformation[i].coordinates->y
            - bus->stopInformation[i+1].coordinates->x - bus->stopInformation[i+1].coordinates->y == 10) {
                pop = 2;
            }
            else {
                pop = 3;
            }

            if (bus->stopInformation[i].stopMin + pop > 60) {
                bus->stopInformation[i+1].stopHour += 1;
                bus->stopInformation[i+1].stopMin = bus->stopInformation[i].stopMin +  pop - 60;
            }
            else {
                bus->stopInformation[i+1].stopMin = bus->stopInformation[i].stopMin + pop;
            }
        }
        street->pastslowdown = street->slowdown;
    }
        return bus;
}
