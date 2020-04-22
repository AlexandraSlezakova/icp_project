#include <iomanip>
#include "Bus.h"
#include "BusRouteMap.h"
#include "../View/Timer.h"
#include "../View/StreetMap.h"

Bus::Bus(int id, int busNumber, Coordinates *position, int busIteration)
{
    id_ = id;
    busNumber_ = busNumber;
    busPosition = position;
    iteration = busIteration;
    roadStopOnRoad = false;
    LoadTimetable();
}

Bus::~Bus()
{
    delete busPosition;
    busPosition = nullptr;
    delete busPhoto;
    busPhoto = nullptr;
}

void
Bus::LoadTimetable()
{
    std::ifstream file;
    std::ostringstream ss;
    std::string line, minute;
    std::vector<std::string> tokens;
    int stopHour, stopMin, stopMinCopy;
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
        stopHour = std::stoi(tokens[0]) + Timer::GetHour();
        stopMinCopy = stopMin = std::stoi(tokens[1]);
        coordinates = Stop::GetStop(tokens[2]);

        if (iteration) {
            stopMin += (iteration * 10);
            if (stopMinCopy >= 50 && stopMinCopy <= 59) {
                stopHour++;
                stopMin = stopMin == 50 ? 0 : stopMin - 60;
                IF(stopHour == 24, stopHour = 0)
            }
        }

        Coordinates::BusStop_S information = {coordinates, stopHour, stopMin, tokens[2]};
        /* save information */
        stopInformation.push_back(information);
    }

    int minuteNow = Timer::GetMinute();
    int hourNow = Timer::GetHour();
    int size = stopInformation.size();
    Coordinates::BusStop_S *info = &stopInformation[size - 1];
    if (minuteNow >= info->stopMin && hourNow == info->stopHour) {
        for (int i = 0; i < size; i++) {
            info = &stopInformation[i];
            info->stopMin += minuteNow;

            if (info->stopMin >= 60) {
                info->stopHour++;
                info->stopMin -= 60;
                IF(info->stopHour == 24, info->stopHour = 0)
            }
        }
    }

    currentBusStop = stopInformation[0];
    nextBusStop = stopInformation[1];

    file.close();
}

void
Bus::CreateTimetable(QString& color, QPlainTextEdit *textArea)
{
    std::string minute;
    int hour, min;
    QFont font;
    font.setPointSize(11);
    textArea->setFont(font);
    textArea->appendPlainText("Route " + QString::number(busNumber_) + " (id: " + QString::number(id_) + ")");

    for (const Coordinates::BusStop_S& info : stopInformation) {
        /* show bus timetable in text area */
        std::ostringstream stream;

        hour = info.stopHour;
        min = info.stopMin;

        minute = std::to_string(min);
        if (minute.size() == 1) {
            minute = "0" + minute;
        }

        stream << std::to_string(hour) << ":" << minute << " " << info.name;
        textArea->appendPlainText(QString::fromStdString(stream.str()));
    }

    /* draw bus route on map */
    BusRouteMap::DrawLine(stopInformation, color);
}

void
Bus::InitBus(QGraphicsScene *scene)
{
    QString path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus.png"));
    busPhoto = new QGraphicsPixmapItem(QPixmap(path));
    busPhoto->setToolTip(QString::fromStdString(std::to_string(busNumber_)));
    busPhoto->setScale(0.06);
    busPhoto->setPos(0, 0 - 5);
    scene->addItem(busPhoto);
}

void
Bus::MoveBus()
{
    int secNow, x, y, xShift = 0, yShift = 0;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int rotation;

    if (minuteNow == nextBusStop.stopMin) {
        deleteBus = 1;
        return;
    }

    secNow = Timer::GetSecond();
    x = currentBusStop.coordinates->x;
    y = currentBusStop.coordinates->y;
    deleteBus = 0;

    rotation = 0;
    if (nextBusStop.coordinates->x > x) {
        busPhoto->setTransform(QTransform::fromScale(1, 1));
    }
    else if (nextBusStop.coordinates->x < x) {
        busPhoto->setTransform(QTransform::fromScale(-1, 1));
    }
    else {
        if (nextBusStop.coordinates->y > y ) {
            rotation = 90;
            busPhoto->setTransform(QTransform::fromScale(1, 1));
        }
        else {
            rotation = -90;
            busPhoto->setTransform(QTransform::fromScale(1, 1));
        }
    }

    busPhoto->setRotation(rotation);

    /* moving along the X axis */
    if (nextBusStop.coordinates->y == y) {
        x = Bus::GetCoordinate(hourNow,minuteNow,secNow, 1, currentBusStop, nextBusStop);
        yShift = -5;

    } /* moving along the Y axis */
    else if (nextBusStop.coordinates->x == x) {
        y = Bus::GetCoordinate(hourNow, minuteNow, secNow, 0, currentBusStop, nextBusStop);
        xShift = currentBusStop.coordinates->y < nextBusStop.coordinates->y ? 25 : -5;
    }

    busPosition->x = x;
    busPosition->y = y;
    busPhoto->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
}

int
Bus::GetCoordinate(int hourNow, int minNow, int secNow, int isC, const Coordinates::BusStop_S& current,
                   const Coordinates::BusStop_S& next)
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

    /* avg bus movement one square for x sec */
    int avgMove = std::nearbyint(timerStop / countSquare);
    /*  time when bus past last stop */
    int moved = 0;

    if (hourNow > current.stopHour)
        minNow += 60;

    if (minNow == current.stopMin)
        moved = secNow;
    else
        moved += ((minNow - currentBusStop.stopMin) * 60) + secNow;

    moved = std::nearbyint(moved / avgMove);

    coordinates = isC ? currentBusStop.coordinates->x + moved : currentBusStop.coordinates->y + moved;

    return coordinates;
}