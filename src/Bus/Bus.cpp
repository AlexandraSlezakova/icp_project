#include <iomanip>
#include "Bus.h"
#include "BusRouteMap.h"
#include "../View/Timer.h"
#include "../View/StreetMap.h"

Bus::Bus(int id, int busNumber, Coordinates *position)
{
    id_ = id;
    busNumber_ = busNumber;
    busPosition = position;
    roadstoponroad = false;
    LoadTimetable();
}

void
Bus::LoadTimetable()
{
    std::ifstream file;
    std::ostringstream ss;
    std::string line, minute;
    std::vector<std::string> tokens;
    int timeFrom;
    Coordinates *coordinates;

    ss << "../files/bus-route-map/" << std::to_string(busNumber_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());
    ss.clear();

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    /* save bus stop
     * each line has own data with hour [0], minutes [1]
     * and name of bus stops [3] */
    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");

        size_t eol = tokens[2].find('\r');
        if( eol != std::string::npos)
            tokens[2] = tokens[2].substr(0,tokens[2].size() - 1);

        timeFrom = std::stoi(tokens[0]) + Timer::GetHour();
        coordinates = Stop::GetStop(tokens[2]);

        Coordinates::BusStop_S information;
        information.coordinates = coordinates;
        information.stopHour = timeFrom;
        information.stopMin = std::stoi(tokens[1]);
        information.name = tokens[2];
        /* save information */
        stopInformation.push_back(information);
    }

    currentBusStop = stopInformation[0];
    nextBusStop = stopInformation[1];

    file.close();
}

void
Bus::CreateTimetable(QPlainTextEdit *text, Square *layout[X][Y], const QString& color)
{
    std::string minute;
    for (const Coordinates::BusStop_S& info : stopInformation) {
        /* show bus timetable in text area */
        std::ostringstream stream;

        minute = std::to_string(info.stopMin);
        if (minute.size() == 1) {
            minute = "0" + minute;
        }

        stream << std::to_string(info.stopHour) << ":" << minute << " " << info.name;
        text->appendPlainText(QString::fromStdString(stream.str()));
    }

    /* draw bus route on map */
    BusRouteMap::DrawLine(stopInformation, layout, color);
}

void
Bus::InitBus(QGraphicsScene *scene)
{
    QString path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus.png"));
    bus = new QGraphicsPixmapItem(QPixmap(path));
    bus->setToolTip(QString::fromStdString(std::to_string(busNumber_)));
    bus->setScale(0.06);
    bus->setPos(0, 0 - 5);
    scene->addItem(bus);
}

void
Bus::MoveBus()
{
    int secNow, x, y, xShift = 0, yShift = 0;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int rotation;
    Coordinates::BusStop_S next, current;

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        current = stopInformation[i];
        next = stopInformation[i + 1];

        if (next.stopHour == hourNow && next.stopMin > minuteNow) {
            secNow = Timer::GetSecond();
            x = current.coordinates->x;
            y = current.coordinates->y;

            rotation = 0;
            if(next.coordinates->x > x) {
                bus->setTransform(QTransform::fromScale(1, 1));
            }
            else if (next.coordinates->x < x) {
                bus->setTransform(QTransform::fromScale(-1, 1));
            }
            else{
                if( next.coordinates->y > y ) {
                    rotation = 90;
                    bus->setTransform(QTransform::fromScale(1, 1));
                }
                else {
                    rotation = -90;
                    bus->setTransform(QTransform::fromScale(1, 1));
                }
            }

            bus->setRotation(rotation);

            /* moving along the X axis */
            if (next.coordinates->y == y) {
                x = Bus::GetCoordinate(hourNow,minuteNow,secNow, 1, current, next);
                yShift = -5;

            } /* moving along the Y axis */
            else if (next.coordinates->x == x) {
                y = Bus::GetCoordinate(hourNow, minuteNow, secNow, 0, current, next);
                xShift = current.coordinates->y < next.coordinates->y ? 25 : -5;
            }

            busPosition->x = x;
            busPosition->y = y;
            bus->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
            break;
        }
    }
}

int
Bus::GetCoordinate(int hourNow, int minNow, int secNow, int isC,
                   const Coordinates::BusStop_S& current,const Coordinates::BusStop_S& next)
{

    int coordinates;
    /* get number square between current and next stop */
    int countSquare = next.coordinates->x + next.coordinates->y - currentBusStop.coordinates->x - currentBusStop.coordinates->y;
    /* time in sec between current a next stop */
    int timerStop;

    if (next.stopMin > minNow)
        timerStop = abs(next.stopMin - currentBusStop.stopMin) * 60 ;
    else
        timerStop = abs( (next.stopMin + 60) - currentBusStop.stopMin) * 60 ;

    /* avg bus movemet  one square for x sec */
    int avgMove = (int)nearbyint(timerStop / countSquare);
    /*  time when bus past last stop */
    int moved = 0;

    if (hourNow > current.stopHour)
        minNow += 60;

    if (minNow == current.stopMin)
        moved = secNow;
    else
        moved += ((minNow - currentBusStop.stopMin) * 60) + secNow;

    moved = nearbyint(moved / avgMove);

    coordinates = isC ? currentBusStop.coordinates->x + moved : currentBusStop.coordinates->y + moved;

    return coordinates;
}


