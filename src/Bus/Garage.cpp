#include "Garage.h"

Garage::Garage() = default;

void
Garage::AddBus(int id, int busNumber, QGraphicsScene *scene, int iteration)
{
    Bus *bus = new Bus(id, busNumber, new Coordinates(0,0), iteration);
    bus->InitBus(scene);
    bus->MoveBus();
    allBuses.push_back(bus);
}

Bus*
Garage::GetBus(int id) {
    for (Bus *bus : allBuses) {
        if (id == bus->id_)
            return bus;
    }

    return nullptr;
}

Bus*
Garage::GetBusByPhoto(QGraphicsItem *photo)
{
    for (Bus *bus : allBuses) {
        if (photo == bus->busPhoto)
            return bus;
    }

    return nullptr;
}

void
Garage::MoveAllBuses(StreetMap *streetMap, QGraphicsScene *scene) {
    for (Bus *bus : allBuses) {
        CheckRoadBlock(bus);
        bus = CheckSlowDown(streetMap, bus);
        bus->MoveBus();

        if (bus->deleteBus) {
            DeleteBus(bus, scene);
        }
    }
}

void
Garage::DeleteBus(Bus *bus, QGraphicsScene *scene)
{
    auto found = std::find(std::begin(allBuses), std::end(allBuses), bus);
    allBuses.erase(found);
    scene->removeItem(bus->busPhoto);
    delete bus;
    /* set to nullptr to avoid crashing on double delete */
    bus = nullptr;
}

void
Garage::DeleteBuses(QGraphicsScene *scene)
{
    Coordinates::BusStop_S stopInformation;
    int busStorageSize = allBuses.size();

    for (int i = 0; i < busStorageSize; i++) {
        Bus *bus = allBuses[i];
        stopInformation = bus->stopInformation[0];
        DeleteBus(bus, scene);
        busStorageSize--;
        i--;
    }
}

bool
Garage::CheckRoadBlock(Bus *bus) {
//    Street *afterNextStreet;
//    int i = 0;
//    for (;i < bus->stopInformation.size() - 2 && bus->stopInformation[i].name != bus->nextBusStop.name; i++) {
//    }
//
//    if (Square::layout[bus->nextBusStop.coordinates->x][bus->nextBusStop.coordinates->y]->roadBlock) {
//        if (bus->nextBusStop.coordinates->x == bus->stopInformation[i].coordinates->x) {
//            if (Square::layout[bus->nextBusStop.coordinates->x][( bus->nextBusStop.coordinates->y + bus->stopInformation[i].coordinates->y ) / 2]->roadBlock) {
//
//            }
//        }
//    }
}

Bus*
Garage::CheckSlowDown(StreetMap *streetMap, Bus *bus) {

    std::vector<std::string> currentSplit;
    std::vector<std::string> nextSplit;
    std::string streetName;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    Street *street;
    int timeAdd;
    int i = 0;
    int stopTime;
    int pop;

    /* find out where the bus should be according to current time */
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
    nextSplit = Functions::Split(bus->stopInformation[i + 1].name, "-");
    /* get name of street where bus is
     * example: Pešinova-Škálova and Pešinova-Karlova
     * - bus is at Pešinova street */
    streetName = currentSplit[0] == nextSplit[0] or currentSplit[0] == nextSplit[1]
            ? currentSplit[0]
            : currentSplit[1];

    street = streetMap->GetStreet(streetName);

    /* change time in timetable
     * used in street slowdown */
    if ((bus->stopInformation[i].name != bus->currentBusStop.name and bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        or street->previousSlowdown != street->slowdown) {

        if (bus->stopInformation[i].name != bus->currentBusStop.name
            && bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        {
            bus->currentBusStop = bus->stopInformation[i];
            bus->nextBusStop = bus->stopInformation[i + 1];
        }
        else {
             bus->currentBusStop.coordinates.x = bus->busPosition->x;
             bus->currentBusStop.coordinates.y = bus->busPosition->y;
             bus->currentBusStop.stopHour = hourNow;
             bus->currentBusStop.stopMin = minuteNow;
        }


        if (street->previousSlowdown != street->slowdown) {
            /* how many minutes the bus is already on the road */
            if (minuteNow < bus->currentBusStop.stopMin)
                timeAdd = 60 + minuteNow - bus->currentBusStop.stopMin;
            else
                timeAdd = minuteNow - bus->currentBusStop.stopMin;

            /* time affected by slowdown */
            if (bus->nextBusStop.stopMin < bus->currentBusStop.stopMin)
                stopTime = 60 +  bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;
            else
                stopTime = bus->nextBusStop.stopMin - bus->currentBusStop.stopMin - timeAdd;

            /* street slowdown */
            stopTime = round(stopTime * street->slowdown);

            /* change time in timetable of next bus stop
             * time in timetable of previous bus stop + time of the bus on the road
             * + the remaining time needed to arrive at the next bus stop */
            if(bus->currentBusStop.stopMin + stopTime > 60) {
                bus->stopInformation[i + 1].stopHour += 1;
                bus->stopInformation[i + 1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime - 60;
            }
            else {
                bus->stopInformation[i + 1].stopMin = bus->currentBusStop.stopMin + timeAdd + stopTime;
            }

            i++;

            /* change of time in timetable
             * 10 squares on the map  - 2 minutes
             * 19 squares on the map - 3 minutes
             * the delay is already counted from the previous calculation
             * - example:
             * time in timetable of current bus stop - 10:02
             * and next bus stop - 10:04
             * bus is on the road for a minute and one minute left
             * value of slowdown - 2
             * so 2 minutes left
             * new calculated value: 10:02 + traveled distance(1) + delay(2)
             * = new time 10:05 */
            for (; i < bus->stopInformation.size() - 1; i++) {
                if (bus->stopInformation[i].coordinates.x + bus->stopInformation[i].coordinates.y
                    - bus->stopInformation[i + 1].coordinates.x - bus->stopInformation[i + 1].coordinates.y == 10) {
                    pop = 2;
                }
                else {
                    pop = 3;
                }

                if (bus->stopInformation[i].stopMin + pop >= 60) {
                    bus->stopInformation[i + 1].stopHour += 1;
                    bus->stopInformation[i + 1].stopMin = bus->stopInformation[i].stopMin +  pop - 60;
                }
                else {
                    if (bus->stopInformation[i + 1].stopHour < bus->stopInformation[i].stopHour) {
                        bus->stopInformation[i + 1].stopHour += 1;
                    }
                    bus->stopInformation[i + 1].stopMin = bus->stopInformation[i].stopMin + pop;
                }
            }
        }

        /* current slowdown */
        street->previousSlowdown = street->slowdown;
    }

    std::vector<std::string>().swap(currentSplit);
    std::vector<std::string>().swap(nextSplit);

    return bus;
}
