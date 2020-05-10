#include "Bus.h"

Bus::Bus(int id, int busNumber, int busCount)
{
    id_ = id;
    busNumber_ = busNumber;
    busPosition.x = 0;
    busPosition.y = 0;
    roadStopOnRoad = false;
    pastStops = 0;
    LoadTimetable(busCount);
}

Bus::~Bus()
{
    if (busPhoto != nullptr) {
        delete busPhoto;
        busPhoto = nullptr;
    }
}

void
Bus::LoadTimetable(int busCount)
{
    std::ifstream file;
    std::ostringstream ss;
    std::string line, minute;
    std::vector<std::string> tokens;
    int stopHours, stopMinutes, id = 0;
    Coordinates::Coordinates_S coordinates;

    ss << "../examples/bus-route-map/" << std::to_string(busNumber_) << ".txt";
    std::string path = Functions::GetAbsolutePath(ss.str().c_str());
    ss.clear();

    file.open(path);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    int minuteNow = Timer::GetMinute();
    int hourNow = Timer::GetHour();
    int timeInMinute = hourNow * 60 + minuteNow;
    /* time divisible by fifteen */
    int startTime = timeInMinute - (timeInMinute % 15);

    int newStartTime = 0;
    bool firstStop = true;

    /* save bus stop
     * each line has own data with hour [0], minutes [1]
     * and name of bus stops [3] */
    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");

        stopHours = std::stoi(tokens[0]);
        stopMinutes = std::stoi(tokens[1]);
        coordinates = StreetMap::GetStopByName(tokens[2]);

        /* newly calculated start time */
        newStartTime = startTime - (busCount * 15) + stopHours * 60 + stopMinutes;

        if (firstStop) {
            if (0 <= hourNow && hourNow < 6) {
                deleteBus = 1;
                return;
            }
            firstStop = false;
        }

        Coordinates::BusStop_S information;
        information.coordinates = coordinates;
        information.stopHour = newStartTime / 60;
        information.stopMin = newStartTime % 60;
        information.name = tokens[2];
        information.id = id;

        /* save information */
        stopInformation.push_back(information);
        id++;
    }

    /* find out where the bus should be according to current time */
    minuteNow = Timer::GetMinute();
    hourNow = Timer::GetHour();

    GetBusStops();

    file.close();
}

void
Bus::CreateTimetable(QString& color, QPlainTextEdit *plainTextArea)
{
    std::string minute;
    int hour, min;
    QFont font;
    font.setPointSize(11);
	plainTextArea->setFont(font);
	plainTextArea->appendPlainText("Route " + QString::number(busNumber_) + " (id: " + QString::number(id_) + ")");

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
	    plainTextArea->appendPlainText(QString::fromStdString(stream.str()));
    }

    /* draw bus route on map */
    BusRouteMap::DrawLine(stopInformation, color);
}

void
Bus::GetBusStops() {
    /* find out where the bus should be according to current time */
    int minuteNow = Timer::GetMinute();
    int hourNow = Timer::GetHour();

    int nxt, nw, mn, i = 0;
    int stopInformationSize = (int)stopInformation.size();
    for (; i < stopInformationSize - 2; i++) {
        nxt = stopInformation[i + 1].stopHour * 60 + stopInformation[i + 1].stopMin;
        nw  = hourNow * 60 + minuteNow;
        mn = stopInformation[i].stopHour * 60 + stopInformation[i].stopMin;

        if (nxt > nw and nw >= mn)
            break;
    }
    pastStops = i;

    currentBusStop = stopInformation[i];
    nextBusStop = stopInformation[i + 1];
}

void
Bus::InitBusPhoto(QGraphicsScene *scene, const char *imagePath, int x, int y)
{
    QString path = QString::fromStdString(Functions::GetAbsolutePath(imagePath));

    if (busPhoto) {
    	delete busPhoto;
    	busPhoto = nullptr;
    }

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

void
Bus::BusRotation(int x, int y, const Coordinates::BusStop_S &next) const {
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

    if (countSquare == 0) {
        return isC ? busPosition.x : busPosition.y;
    }
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