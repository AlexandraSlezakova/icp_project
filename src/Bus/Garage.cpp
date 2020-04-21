
#include "Garage.h"

Garage::Garage(int busId, int busNumber,QGraphicsScene *scene)
{
    Bus *bus = new Bus(busId, busNumber, new Coordinates(0,0));
    bus->InitBus(scene);
    bus->MoveBus();
    line1.push_back(bus);
    allbus.push_back(bus);
}

void
Garage::AddBus(Bus bus)
{

}

Bus* Garage::GetBus(int busId, int busLine) {
    if (busLine == 1) {
        for (auto & bus : line1) {
            if (busId == bus->id_)
                return bus;
        }
    }
    return nullptr;
}

void Garage::MoveAllBuses(StreetMap *streetMap) {
    for (int i = 0; i < allbus.size(); i++ ) {
        //if (allbus[i]->roadstoponroad || )
        CheckRoadBlock(streetMap,allbus[i]);
        allbus[i] = CheckSlowDown(streetMap,allbus[i]);
        allbus[i]->MoveBus();
    }
}

bool
Garage::CheckRoadBlock(StreetMap *streetMap, Bus *bus) {
    Street *afternextstreet;
    int i = 0;
    for(;i < bus->stopInformation.size() - 2 && bus->stopInformation[i].name != bus->nextBusStop.name; i++) {
    }
    if (bus->stopInformation[i].name == bus->nextBusStop.name) {
        std::cerr << "joo \n";
    }
    if (streetMap->layout[bus->nextBusStop.coordinates->x][bus->nextBusStop.coordinates->y]->roadBlock) {
        if(bus->nextBusStop.coordinates->x == bus->stopInformation[i].coordinates->x) {
            if (streetMap->layout[bus->nextBusStop.coordinates->x][( bus->nextBusStop.coordinates->y + bus->stopInformation[i].coordinates->y ) / 2]->roadBlock) {

            }
        }
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

    /* zjištěný na základě času mezi kterými zastávkami se autobuse nalézá */
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

    /*výpočet nového času u zastávky je počítáno jen když se změní:
     * název minulé a budoucí zastávky
     * zpomalení ulice
     * */
    if ((bus->stopInformation[i].name != bus->currentBusStop.name and bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        or street->pastslowdown != street->slowdown) {

        /* rozhodování které změná možnost proběhla */
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

        /* výpočet kolik minut je už bus na cestě */
        if (minuteNow < bus->currentBusStop.stopMin)
            timeAdd = 60 + minuteNow - bus->currentBusStop.stopMin;
        else
            timeAdd = minuteNow - bus->currentBusStop.stopMin;

        /* dopočítání času, který se bude zpomalovat */
        if (bus->nextBusStop.stopMin < bus->currentBusStop.stopMin)
            stopTime = 60 +  bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;
        else
            stopTime = bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;

        /* výpočet zpomalení času */
        stopTime = round(stopTime * street->slowdown);

        /* změna času následující zastávky
         * čas minuté zastávky + čas který autobus ujel + vypočítaného času, který ještě zbývá do další zastávky
         * */
        if(bus->currentBusStop.stopMin + stopTime > 60) {
            bus->stopInformation[i+1].stopHour += 1;
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime - 60;
        }
        else {
            bus->stopInformation[i+1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime;
        }

        i++;

        /* změna všech následujích zastávek o čas na základě vzdálenosti v dílcíhc
         * 10 dílků - 2 minuty
         * 19 dílku - 3 minuty
         * zpoždění je už započítané z předchozího výpočtu
         * example
         * mezi zastávkami jede autobus 2 minuty 10:02, 10:04
         * minutu už ujel
         * minuta zbývá
         * zpomalená ulice 2
         * dopočítaná hodnota 2 minuty
         * nově vypočítaná hodnota 10:02 + ujetá vzdálenost (1) + dopočítané zpoždění (2)
         * = dojezdový čas 10:05
         * */
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
        /* uležní aktuální zpomalení */
        street->pastslowdown = street->slowdown;
    }
        return bus;
}
