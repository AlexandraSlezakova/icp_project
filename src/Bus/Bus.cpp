#include "Bus.h"

Bus::Bus(int id, int busNumber, int busIteration, int routeLen)
{
    id_ = id;
    busNumber_ = busNumber;
    busPosition.x = 0;
    busPosition.y = 0;
    iteration = busIteration;
    routeLength = routeLen;
    roadStopOnRoad = false;
    LoadTimetable();
}

Bus::~Bus()
{
    std::vector<Coordinates::busStop>().swap(stopInformation);

    if (busPhoto) {
        delete busPhoto;
        busPhoto = nullptr;
    }
}

void
Bus::LoadTimetable()
{
    std::ifstream file;
    std::ostringstream ss;
    std::string line, minute;
    std::vector<std::string> tokens;
    int stopHours, stopMinutes, minutes, id = 0;
    Coordinates::Coordinates_S coordinates;

    ss << "../examples/bus-route-map/" << std::to_string(busNumber_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());
    ss.clear();

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    /* save bus stop
     * each line has own data with hour [0], minutes [1]
     * and name of bus stops [3] */
    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");

        stopHours = std::stoi(tokens[0]) + Timer::GetHour();
        stopMinutes = std::stoi(tokens[1]);
        coordinates = StreetMap::GetStopByName(tokens[2]);

        if (iteration > 0) {
            stopMinutes += (iteration * 15);
            if (stopMinutes >= 60) {
                stopHours++;
                stopMinutes -= 60;
            }
        }
        else if (iteration < 0) {
            /* new time with negative iteration */
            minutes = (stopHours * 60 + stopMinutes) + (iteration * 15);
            stopHours = minutes / 60;
            stopMinutes = minutes - (stopHours * 60);
        }

        Coordinates::BusStop_S information;
        information.coordinates = coordinates;
        information.stopHour = stopHours;
        information.stopMin = stopMinutes;
        information.name = tokens[2];
        information.id = id;

        /* save information */
        stopInformation.push_back(information);
        id++;
    }
    std::vector<std::string>().swap(tokens);

    int minuteNow = Timer::GetMinute();
    int hourNow = Timer::GetHour();
    int size = stopInformation.size();
    Coordinates::BusStop_S *info = &stopInformation[size - 1];

    if (minuteNow >= info->stopMin && hourNow == info->stopHour) {
        if (iteration < 0) {
            deleteBus = 1;
        }
        else {
            while (minuteNow % 15) {
                minuteNow++;
            }

            for (int i = 0; i < size; i++) {
                info = &stopInformation[i];
                info->stopMin += minuteNow;

                /* bus with arrival time at the first bus stop hourNow:00 already exists
                 * if time of current bus has same arrival time, delete bus */
                if (i == 0 && info->stopMin == 0 && info->stopHour == hourNow) {
                    deleteBus = 1;
                    break;
                }

                if (info->stopMin >= 60) {
                    info->stopHour++;
                    info->stopMin -= 60;
                }

                if ((info->stopMin > minuteNow || info->stopHour > hourNow) && i == 0 && !iteration) {
                    i = iteration = -1;
                    minuteNow -= 15;
                    IF(info->stopHour > hourNow, info->stopHour--)
                }
            }
        }
    }

    currentBusStop = stopInformation[0];
    nextBusStop = stopInformation[1];

    /* buses with arrival time at first bus stops greater than midnight are deleted */
    int midnight = 23 * 60 + 60;
    stopHours = stopInformation[size - 1].stopHour;
    IF(stopHours >= 0 && stopHours < 6, stopHours += 24)
    int firstBusStop = stopHours * 60 + stopInformation[size - 1].stopMin;
    IF(firstBusStop >= midnight, deleteBus = 1)

    /* save route length in minutes */
    if (iteration == 0) {
        Coordinates::BusStop_S lastBusStop = stopInformation[size - 1];
        routeLength = (lastBusStop.stopHour * 60 + lastBusStop.stopMin)
                      - (currentBusStop.stopHour * 60 + currentBusStop.stopMin);
    }

    /* last check if bus arrival time at its first bus stop
     * is less or equal to current time */
    if (!deleteBus) {
        if (currentBusStop.stopMin > minuteNow || currentBusStop.stopHour > hourNow) {
            deleteBus = 1;
        }
    }

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
Bus::InitBus(QGraphicsScene *scene, const char *imagePath, int x, int y)
{
    QString path = QString::fromStdString(Functions::GetAbsolutePath(imagePath));
    busPhoto = new QGraphicsPixmapItem(QPixmap(path));
    busPhoto->setToolTip(QString::fromStdString(std::to_string(busNumber_)));
    busPhoto->setScale(0.06);
    busPhoto->setPos(x, y - 5);
    scene->addItem(busPhoto);
}

void
Bus::MoveBus()
{
    int secNow, x, y, xShift = 0, yShift = 0;
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int stopInfoSize = (int)stopInformation.size() - 1;

    if (minuteNow >= nextBusStop.stopMin && nextBusStop.id == stopInfoSize) {
        deleteBus = 1;
        return;
    }

    secNow = Timer::GetSecond();
    x = currentBusStop.coordinates.x;
    y = currentBusStop.coordinates.y;
    deleteBus = 0;

    BusRotation(x, y, nextBusStop);

    /* moving along the X axis */
    if (nextBusStop.coordinates.y == y) {
        x = Bus::GetCoordinate(hourNow, minuteNow, secNow, 1, currentBusStop, nextBusStop);
        yShift = -5;

    } /* moving along the Y axis */
    else if (nextBusStop.coordinates.x == x) {
        y = Bus::GetCoordinate(hourNow, minuteNow, secNow, 0, currentBusStop, nextBusStop);
        xShift = currentBusStop.coordinates.y < nextBusStop.coordinates.y ? 25 : -5;
    }

    busPosition.x = x;
    busPosition.y = y;
    busPhoto->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
}

void Bus::BusRotation(int x, int y, const Coordinates::BusStop_S &next) const {
    int rotation = 0;
    if (next.coordinates.x > x) {
        busPhoto->setTransform(QTransform::fromScale(1, 1));
    }
    else if (next.coordinates.x < x) {
        busPhoto->setTransform(QTransform::fromScale(-1, 1));
    }
    else {
        if (next.coordinates.y > y ) {
            rotation = 90;
            busPhoto->setTransform(QTransform::fromScale(1, 1));
        }
        else {
            rotation = -90;
            busPhoto->setTransform(QTransform::fromScale(1, 1));
        }
    }

    busPhoto->setRotation(rotation);
}

int
Bus::GetCoordinate(int hourNow, int minNow, int secNow, int isC, const Coordinates::BusStop_S& current,
                   const Coordinates::BusStop_S& next)
{

    int coordinates;
    /* get number square between current and next stop */
    int countSquare = next.coordinates.x + next.coordinates.y - currentBusStop.coordinates.x - currentBusStop.coordinates.y;
    /* time in sec between current a next stop */
    int timerStop;

    if (next.stopMin > minNow)
        timerStop = abs(next.stopMin - currentBusStop.stopMin) * 60 ;
    else
        timerStop = abs( (next.stopMin + 60) - currentBusStop.stopMin) * 60 ;

    /* avg bus movement one square for x sec */
    int avgMove = !countSquare ? 11 : std::nearbyint(timerStop / countSquare);
    /*  time when bus past last stop */
    int moved = 0;

    if (hourNow > current.stopHour)
        minNow += 60;

    if (minNow == current.stopMin)
        moved = secNow;
    else
        moved += ((minNow - currentBusStop.stopMin) * 60) + secNow;

    moved = std::nearbyint(moved / avgMove);

    coordinates = isC ? currentBusStop.coordinates.x + moved : currentBusStop.coordinates.y + moved;

    return coordinates;
}