
#include "Garage.h"

Garage::Garage(int busId, int busNumber,QGraphicsScene *scene)
{
    auto *pos = new Coordinates(0,0);
    Bus *bus = new Bus(busId, busNumber, pos);
    bus->InitBus(scene);
    bus->MoveBus();
    line1.push_back(bus);
    //allBusses.push_back(line1);
}

void Garage::AddBus(Bus bus) {

}

Bus* Garage::GetBus(int busId, int busLine) {
    if (busLine == 1)
    {
        for ( int i = 0; i < line1.size(); i++ )
            if (busId == line1[i]->id_)
                return line1[i];
    }
    return nullptr;
}

void Garage::MoveAllBusses(StreetMap *streetMap) {
    for ( int i = 0; i < line1.size(); i++ )
    {

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

    std::vector<std::string> currentsplit;
    std::vector<std::string> nextsplit;
    std::string streetname = "a";
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int secNow = Timer::GetSecond();
    Street *street;
    int timeadd = 0;
    int i = 0;
    int stopTime = 0;

    for (; i < bus->stopInformation.size() - 2; i++) {
        int nxt = bus->stopInformation[i+1].stopHour * 60 + bus->stopInformation[i+1].stopMin;
        int nw  = hourNow * 60 + minuteNow ;
        int mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;
        if (nxt > nw and nw >= mn)
            break;
    }

    if(bus->stopInformation[i].name == "")
        return bus;

    currentsplit = Functions::Split(bus->stopInformation[i].name, "-");
    nextsplit = Functions::Split(bus->stopInformation[i+1].name, "-");



    if (currentsplit[0] == nextsplit[0] or currentsplit[0] == nextsplit[1])
        streetname = currentsplit[0];
    else
        streetname = currentsplit[1];

    if (streetname == "a")
        std::cerr << "Error: Street name wasn't found" << std::endl;

    street = streetMap->GetStreet(streetname);

    if((bus->stopInformation[i].name != bus->currentBusStop.name and bus->stopInformation[i + 1].name != bus->nextBusStop.name) or street->pastslowdown != street->slowdown)
    {
        if(bus->stopInformation[i].name != bus->currentBusStop.name && bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        {
            bus->currentBusStop = bus->stopInformation[i];
            bus->nextBusStop = bus->stopInformation[i + 1];
        }
        else{
            bus->currentBusStop.coordinates = bus->busPosition;
        }


        if ( minuteNow < bus->currentBusStop.stopMin )
            timeadd = 60 + minuteNow - bus->currentBusStop.stopMin;
        else
            timeadd = minuteNow - bus->currentBusStop.stopMin;

        if(bus->nextBusStop.stopMin < bus->currentBusStop.stopMin )
            stopTime = 60 + bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeadd;
        else
            stopTime = bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeadd;

        stopTime = round(stopTime * street->slowdown);

        if(bus->currentBusStop.stopMin + stopTime > 60)
        {
            bus->stopInformation[i+1].stopHour += 1;
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeadd + stopTime - 60;
        }
        else
        {
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeadd + stopTime;
        }

        std::cerr<< "CHECK time next " << bus->stopInformation[i+1].name << " = " << bus->stopInformation[i+1].stopMin<< " \n";
        i++;

        for (; i < bus->stopInformation.size() - 2; i++) {
            if(bus->currentBusStop.stopMin + stopTime > 60)
            {
                bus->stopInformation[i+1].stopHour += 1;
                bus->stopInformation[i+1].stopMin = bus->stopInformation[i].stopMin +  stopTime - 60;
            }
            else
            {
                bus->stopInformation[i+1].stopMin = bus->stopInformation[i].stopMin + stopTime;
            }
        }
        street->pastslowdown = street->slowdown;

    }
        return bus;
}
