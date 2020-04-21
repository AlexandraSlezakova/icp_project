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
    roadStopOnRoad = false;
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
Bus::CreateTimetable(QString& color)
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
        textArea->appendPlainText(QString::fromStdString(stream.str()));
    }

    /* draw bus route on map */
    BusRouteMap::DrawLine(stopInformation, color);
}

void
Bus::ClearTextArea()
{
    textArea->clear();
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
    Coordinates::BusStop_S next, current;

    if (iteration) {
        if (!minuteNow) {
            hourNow--;
            minuteNow = 60;
        }
        else if (minuteNow > 0 && minuteNow < 10) {
            hourNow--;
            minuteNow += 60;
        }
        minuteNow -= (iteration * 10);
    }

    for (int i = 0; i < stopInformation.size() - 1; i++) {
        current = stopInformation[i];
        next = stopInformation[i + 1];

        if (next.stopHour == hourNow && next.stopMin > minuteNow) {
            secNow = Timer::GetSecond();
            x = current.coordinates->x;
            y = current.coordinates->y;

            rotation = 0;
            if (next.coordinates->x > x) {
                busPhoto->setTransform(QTransform::fromScale(1, 1));
            }
            else if (next.coordinates->x < x) {
                busPhoto->setTransform(QTransform::fromScale(-1, 1));
            }
            else {
                if( next.coordinates->y > y ) {
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
            busPhoto->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
            break;
        }
    }
}

int
Bus::GetCoordinate(int hourNow, int minNow, int secNow, int isC, const Coordinates::BusStop_S& current,const Coordinates::BusStop_S& next)
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

QPlainTextEdit *Bus::textArea;

void
Bus::InitTimetableArea(QWidget *parent, int width, int height)
{
    /* text area for bus timetable */
    textArea = new QPlainTextEdit(parent);
    textArea->setMinimumSize(width * 0.19, height * 0.6);
    textArea->move(5, 80);
}