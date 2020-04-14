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
    rotation = 0;
    avgspeed = 11;

    std::ifstream file;
    std::ostringstream ss;
    std::string line;
    std::vector<std::string> tokens;
    int timeFrom;
    Coordinates *coordinates;

    ss << "../files/bus-route-map/" << std::to_string(busNumber_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());
    ss.clear();

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    /* save busstop to array [count], witch each line has own array with hour [0] minutes [1] and name of bus stops [3] */
    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");

        size_t eol = tokens[2].find('\r');
        if( eol != std::string::npos)
            tokens[2]=tokens[2].substr(0,tokens[2].size()-1);

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

    currentBusStop.coordinates = new Coordinates(stopInformation[0].coordinates->x, stopInformation[0].coordinates->y);
    currentBusStop.stopHour = stopInformation[0].stopHour;
    currentBusStop.stopMin = stopInformation[0].stopMin;
    currentBusStop.name = stopInformation[0].name;

    nextBusStop.coordinates = new Coordinates(stopInformation[1].coordinates->x, stopInformation[1].coordinates->y);
    nextBusStop.stopHour = stopInformation[1].stopHour;
    nextBusStop.stopMin = stopInformation[1].stopMin;
    nextBusStop.name = stopInformation[1].name;

    file.close();
}

void
Bus::CreateTimetable(QPlainTextEdit *text, Square *layout[X][Y], const QString& color)
{
    Coordinates *coordinates;
    std::ifstream file;
    std::string line;
    std::vector<std::string> tokens;
    int timeFrom;

    /* get file path */
    std::ostringstream ss;
    ss << "../files/bus-route-map/" << std::to_string(busNumber_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());
    ss.clear();

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");

        // načítalo se mi rádek i s \r, odstranění \r
        size_t eol = tokens[2].find('\r');
        if( eol != std::string::npos)
            tokens[2]=tokens[2].substr(0,tokens[2].size()-1);

        coordinates = Stop::GetStop(tokens[2]);
        timeFrom = std::stoi(tokens[0]) + Timer::GetHour();


        if (!coordinates) {
            std::cerr << "Error: bus stop -- " << tokens[2] << " -- wasn't found" << std::endl;
        } else {
            /* information about bus stop */
            Coordinates::BusStop_S information;
            information.coordinates = coordinates;
            information.stopHour = timeFrom;
            information.stopMin = std::stoi(tokens[1]);
            /* save information */
            stopInformation.push_back(information);
        }
        /* show bus timetable in text area */
        std::ostringstream stream;
        stream << std::to_string(timeFrom) << ":" << tokens[1] << " " << tokens[2];
        text->appendPlainText(QString::fromStdString(stream.str()));
    }

    file.close();

    /* draw lines on map */
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
    int secNow, halfWay, x, y, isX = 0, isY = 0, xShift = 0, yShift = 0;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    Coordinates::BusStop_S next, current;

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        current = stopInformation[i];
        next = stopInformation[i + 1];


        if (next.stopHour == hourNow && next.stopMin > minuteNow) {
            secNow = Timer::GetSecond();
            x = current.coordinates->x;
            y = current.coordinates->y;

            if(next.coordinates->x > x)
            {
                rotation = 0;
                bus->setTransform(QTransform::fromScale(1, 1));
            }
            else if (next.coordinates->x < x)
            {
                rotation = 0;
                bus->setTransform(QTransform::fromScale(-1, 1));
            }
            else{
                if( next.coordinates->y > y )
                {
                    rotation = 90;
                    bus->setTransform(QTransform::fromScale(1, 1));
                }
                else
                {
                    rotation = -90;
                    bus->setTransform(QTransform::fromScale(1, 1));
                }
            }

            bus->setRotation(rotation);


            /* moving along the X axis */
            if (next.coordinates->y == y)
            {
                x = Bus::GetCoordinate(hourNow,minuteNow,secNow, 1, current, next);
                yShift = -5;

            } /* moving along the Y axis */
            else if (next.coordinates->x == x){
                y = Bus::GetCoordinate(hourNow, minuteNow, secNow, 0, current, next);

                if(current.coordinates->y < next.coordinates->y)
                    xShift = +25;
                else
                    xShift = -5;
            }
            busPosition->x = x;
            busPosition->y = y;
            bus->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
            break;
        }
    }
}

int
Bus::GetCoordinate(int hourNow, int minNow, int secNow, int isC, Coordinates::BusStop_S current,Coordinates::BusStop_S next)
{

    /* Get number square between current a and next stop */
     int countSquare = next.coordinates->x + next.coordinates->y - currentBusStop.coordinates->x - currentBusStop.coordinates->y;
    /* Time in sex between current a next stop */
    int timerStop = 0;


    std::cerr<< "BUS MOVE time current " << current.name << " = " << current.stopMin<< " \n";
    std::cerr<< "BUS MOVE time next " << next.name << " = " << next.stopMin<< " \n";

    if ( next.stopMin > minNow)
        timerStop = abs(next.stopMin - currentBusStop.stopMin) * 60;
    else
        timerStop = abs( (next.stopMin + 60) - currentBusStop.stopMin) * 60;

    /* avg bus movomet  one square for x sex */
    int avgMove = timerStop / countSquare;

    /*  time when bus past last stop */
    int moved = 0;

    if (hourNow > current.stopHour)
        minNow += 60;

    if (minNow == current.stopMin)
        moved = secNow;
    else
        moved += ((minNow - current.stopMin) * 60) + secNow;

    moved = moved / avgMove;

    if(isC)
        return current.coordinates->x + moved;
    else
        return current.coordinates->y + moved;





}


