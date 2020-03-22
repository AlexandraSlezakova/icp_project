#include "Bus.h"
#include "BusRouteMap.h"
#include "../View/Timer.h"

Bus::Bus(int id, int busNumber)
{
    id_ = id;
    busNumber_ = busNumber;
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

            /* get halfway of path between 2 bus stops */
            if (next.coordinates->x == x) {
                halfWay = next.coordinates->y > y ? next.coordinates->y - y : y - next.coordinates->y;
                isY = 1;
            } else {
                halfWay = next.coordinates->x > x ? next.coordinates->x - x : x - next.coordinates->x;
                isX = 1;
            }
            halfWay /= 2;

            /* moving along the X axis */
            if (isX) {
                IF(next.coordinates->x < current.coordinates->x, bus->setTransform(QTransform::fromScale(-1, 1));)
                yShift = -5;
            } /* moving along the Y axis */
            else {
                IF_ELSE(current.coordinates->y < next.coordinates->y, bus->setRotation(90),
                        bus->setRotation(-90))
                xShift = -8;
            }

            x = Bus::GetCoordinate(secNow, isX, halfWay, current.coordinates->x, next.coordinates->x);
            y = Bus::GetCoordinate(secNow, isY, halfWay, current.coordinates->y, next.coordinates->y);


            bus->setPos(x * SQUARE_SIZE + xShift, y * SQUARE_SIZE + yShift);
            break;
        }
    }
}

int
Bus::GetCoordinate(int secNow, int isC, int halfWay, int current, int next)
{
    int coordinate = next;

    if (isC && secNow > 30) {
        IF_ELSE(coordinate < current, coordinate = next + halfWay - (halfWay / 2),
                coordinate = next - halfWay + (halfWay / 2))
    } else if (isC && secNow < 30) {
        IF_ELSE(coordinate < current, coordinate = next + halfWay + (halfWay / 2),
                coordinate = next - halfWay - (halfWay / 2))
    }

    return coordinate;
}
