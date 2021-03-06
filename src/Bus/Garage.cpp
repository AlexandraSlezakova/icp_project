#include "Garage.h"

Garage::Garage() = default;

void
Garage::AddBus(int id, int busNumber, QGraphicsScene *scene, int busCount)
{
    Bus *bus = new Bus(id, busNumber, busCount);
    if (bus->deleteBus) {
        delete bus;
        return;
    }

    bus->InitBusPhoto(scene,"../images/bus.png",0,0);
    bus->MoveBus();

    allBuses.push_back(bus);
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
Garage::MoveAllBuses(StreetMap *streetMap, QGraphicsScene *scene)
{
    deletedBus = 0;
    for (Bus *bus : allBuses) {
        if (!bus->stopMoving) {
            bus = CheckRoad(streetMap, bus);
            if (!bus->stopMoving) {
                bus->MoveBus();

                if (bus->deleteBus) {
                    deletedBus = 1;
                    DeleteBus(bus, scene);
                    continue;
                }
            }
        }
        else {
            /* bus line is changed
             * checking for bus stops on new bus line
             * adding delay while the bus stood*/
            if (!CheckRoadBlockShortDistance(bus)) {
                int hourNow = Timer::GetHour();
                int minuteNow = Timer::GetMinute();

                int nxt, nw, mn, i = 0;
                int stopInformationSize = (int)bus->stopInformation.size();
                for (; i < stopInformationSize - 2; i++) {
                    nxt = bus->stopInformation[i + 1].stopHour * 60 + bus->stopInformation[i + 1].stopMin;
                    nw  = hourNow * 60 + minuteNow;
                    mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

                    if (nxt > nw and nw >= mn)
                        break;
                }

                int minAdd = 1 + (hourNow * 60 + minuteNow) - bus->stopInformation[i].stopHour * 60
                        - bus->stopInformation[i].stopMin;

                Coordinates::busStop addStop = bus->stopInformation[i];

                bus->stopInformation.insert(bus->stopInformation.begin() + i, addStop);
                i++;
                bus->pastStops = i;

                stopInformationSize = (int)bus->stopInformation.size();
                /* add delay for all next stops */
                for (; i < stopInformationSize; i++) {
                    if (bus->stopInformation[i].stopMin + minAdd >= 60) {
                        bus->stopInformation[i].stopHour = hourNow + 1;
                        bus->stopInformation[i].stopMin = bus->stopInformation[i].stopMin + minAdd - 60;
                    }
                    else {
                        bus->stopInformation[i].stopHour = bus->stopInformation[i - 1].stopHour;
                        bus->stopInformation[i].stopMin = bus->stopInformation[i].stopMin + minAdd;
                    }
                }

                bus->GetBusStops();
                bus->stopMoving = false;
                bus->roadStopOnRoad = CheckRoadBlockLongDistance(bus);

                /* show black photo of the bus */
                 if (!bus->roadStopOnRoad) {
                     scene->removeItem(bus->busPhoto);
                     bus->InitBusPhoto(scene, "../images/bus.png", bus->busPosition.x * 20, bus->busPosition.y * 20);
                     bus->BusRotation(bus->busPosition.x, bus->busPosition.y, bus->nextBusStop);
                     int yShift = 0;
                     int xShift = 0;
                     /* moving along the X axis */
                     if (bus->nextBusStop.coordinates.y == bus->currentBusStop.coordinates.y) {
                         yShift = -5;

                     } /* moving along the Y axis */
                     else if (bus->nextBusStop.coordinates.x == bus->currentBusStop.coordinates.x) {
                         xShift = bus->currentBusStop.coordinates.y < bus->nextBusStop.coordinates.y ? 25 : -5;
                     }

                     bus->busPhoto->setPos(bus->busPosition.x * SQUARE_SIZE + xShift, bus->busPosition.y * SQUARE_SIZE + yShift);
                 }
            }
        }
        if (bus->roadStopOnRoad) {
            /* bus photo flashing black and red */
            int secNow = Timer::GetSecond();
            const char *imagePath;

            scene->removeItem(bus->busPhoto);
            imagePath = !(secNow % 2) ? "../images/bus.png" : "../images/busWarning.png";

            bus->InitBusPhoto(scene, imagePath, bus->busPosition.x * 20, bus->busPosition.y * 20);
            bus->BusRotation(bus->busPosition.x, bus->busPosition.y, bus->nextBusStop);

            int yShift = 0;
            int xShift = 0;
            /* moving along the X axis */
            if (bus->nextBusStop.coordinates.y == bus->currentBusStop.coordinates.y) {
                yShift = -5;

            } /* moving along the Y axis */
            else if (bus->nextBusStop.coordinates.x == bus->currentBusStop.coordinates.x) {
                xShift = bus->currentBusStop.coordinates.y < bus->nextBusStop.coordinates.y ? 25 : -5;
            }

            bus->busPhoto->setPos(bus->busPosition.x * SQUARE_SIZE + xShift, bus->busPosition.y * SQUARE_SIZE + yShift);
        }
    }
}

bool
Garage::CheckRoadBlockShortDistance(Bus *bus)
{
    /* checking the bus for roadblocks */
    if (bus->roadStopOnRoad) {

        /* check next bustop if it is closed and route behind it
         * example:
         *                     stop             stop
         *  S   R   B   R   R   S   R   R   R   S
         *         bus         |check section|
         * */

        /* if the next stop is closed, it should never get to if statement, it should always jump to else */
        if (Square::layout[bus->currentBusStop.coordinates.x][bus->currentBusStop.coordinates.y]->roadBlock) {
            return false;
        }
        else {
            /* checking second stop if the stop is closed */
            if (Square::layout[bus->nextBusStop.coordinates.x][bus->nextBusStop.coordinates.y]->roadBlock) {
                return true;
            }
            else {
                /* check vertically or horizontally street if it is closed */
                if (bus->currentBusStop.coordinates.x == bus->nextBusStop.coordinates.x) {
                    return Square::layout[bus->currentBusStop.coordinates.x][(bus->currentBusStop.coordinates.y + bus->nextBusStop.coordinates.y) / 2]->roadBlock;
                }
                else {
                    return Square::layout[(bus->currentBusStop.coordinates.x + bus->nextBusStop.coordinates.x) / 2][bus->currentBusStop.coordinates.y]->roadBlock;
                }
            }
        }
    }
    return false;
}

void
Garage::DeleteBus(Bus *bus, QGraphicsScene *scene, int clearStorage)
{
	if (clearStorage) {
		auto found = std::find(std::begin(allBuses), std::end(allBuses), bus);
		allBuses.erase(found);

		/* clear text area, if timetable displayed there belongs to the deleted bus */
		if (bus->textArea) {
			bus->textArea->clear();
			BusRouteMap::DrawLine(bus->stopInformation, "#c0c0c0");
		}
	}
    scene->removeItem(bus->busPhoto);
    delete bus;
}

void
Garage::DeleteBuses(QGraphicsScene *scene)
{
    for (auto bus : allBuses) {
	    DeleteBus(bus, scene, 0);
    }
	std::vector<Bus*>().swap(allBuses);
}

bool
Garage::CheckRoadBlockLongDistance(Bus *bus)
{
    int i = bus->pastStops;
    int stopInformationSize = (int)bus->stopInformation.size();

    /* check next bustop if it is closed and route behind it to the end of bus route
     * example (1. iteration):
     *                     stop             stop
     *  S   R   B   R   R   S   R   R   R   S
     *         bus         |check section|
     * */


    for (; i < stopInformationSize - 1; i++) {
        if (Square::layout[bus->stopInformation[i].coordinates.x][bus->stopInformation[i].coordinates.y]->roadBlock) {
            /* roadStop on stop */
            bus->roadStopOnRoad = true;
            return false;
        }
        /* street going vertically */
        if (bus->stopInformation[i].coordinates.x == bus->stopInformation[i + 1].coordinates.x ) {
            if (Square::layout[bus->stopInformation[i].coordinates.x][(bus->stopInformation[i].coordinates.y + bus->stopInformation[i + 1].coordinates.y) / 2 ]->roadBlock) {
                /* roadStop */
                bus->roadStopOnRoad = true;
                return false;
            }
        }
            /* street going vertically */
        else {
            if (Square::layout[(bus->stopInformation[i].coordinates.x + bus->stopInformation[i + 1].coordinates.x) / 2 ][bus->stopInformation[i].coordinates.y]->roadBlock) {
                /* roadStop */
                bus->roadStopOnRoad = true;
                return false;
            }
        }
    }
    bus->roadStopOnRoad = false;
    return true;
}

Bus*
Garage::CheckRoad(StreetMap *streetMap, Bus *bus) 
{
    std::vector<std::string> currentSplit;
    std::vector<std::string> nextSplit;
    std::string streetName;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    std::shared_ptr<Street> street;
    int timeAdd;
    int i = 0;
    int stopTime;
    int pop;

    /* find out where the bus should be according to current time */
    int nxt, nw, mn;
    int stopInformationSize = (int)bus->stopInformation.size();
    for (; i < stopInformationSize - 2; i++) {
        nxt = bus->stopInformation[i+1].stopHour * 60 + bus->stopInformation[i+1].stopMin;
        nw  = hourNow * 60 + minuteNow;
        mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

        if (nxt > nw and nw >= mn)
            break;
    }

    if (bus->stopInformation.empty())
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
    if ((bus->stopInformation[i].name != bus->currentBusStop.name || bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        or street->previousSlowdown != street->slowdown) {

        if (bus->stopInformation[i].name != bus->currentBusStop.name
            || bus->stopInformation[i + 1].name != bus->nextBusStop.name)
        {
            bus->currentBusStop = bus->stopInformation[i];
            bus->nextBusStop = bus->stopInformation[i + 1];
            bus->pastStops++;

            /* if in short distance is something closed and the bus would get into a dead end
             * save actual time and stop moving with bus*/
            if (CheckRoadBlockShortDistance(bus)) {
                bus->stopMoving = true;
                bus->stopHour = hourNow;
                bus->stopMin = minuteNow;
                return bus;
            }

        }
        else {
             bus->currentBusStop.coordinates.x = bus->busPosition.x;
             bus->currentBusStop.coordinates.y = bus->busPosition.y;
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
            for (; i < stopInformationSize - 1; i++) {
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

    return bus;
}
