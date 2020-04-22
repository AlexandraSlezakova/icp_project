#include "Scene.h"

Scene::Scene(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    SetUpView();
    CreateMap();
}

void
Scene::CreateMap()
{
    Timer::GetTime();
    /* initialize map */
    map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets(Functions::GetAbsolutePath("../files/ulice.txt"));
    map->AddStops(Functions::GetAbsolutePath("../files/zastavky.txt"), scene);
    /* add squares to scene */
    AddSquares();
    /* add buses */
    AddBuses();
}

void
Scene::AddBuses(int iteration)
{
    static int busId = 0;
    //garage.AddBus(busId++, 1, scene, iteration);
    garage.AddBus(busId++, 2, scene, iteration);
}

void
Scene::AddSquares()
{
    for (auto &x : Square::layout) {
        for (auto &y : x) {
            scene->addItem(y);
        }
    }
}

void
Scene::SetUpView()
{
    setFixedSize(X * SQUARE_SIZE, Y * SQUARE_SIZE);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
}

void
Scene::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;

    if (event->delta() > 0) {
        scale(scaleFactor,scaleFactor);
        zoom_act *= scaleFactor;
    }
    else {
        scale(1/scaleFactor,1/scaleFactor);
        zoom_act = zoom_act*(1/scaleFactor);
    }

    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void
Scene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_moving = true;
        m_originalX = event->x();
        m_originalY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    else if (event->button() == Qt::LeftButton) {
        QGraphicsItem *item = itemAt(event->pos());
        auto *square = dynamic_cast<Square*>(item);
        auto *photo = dynamic_cast<QGraphicsPixmapItem*>(item);

        if (roadBlockMode) {
            if (square) {
                SquareRoadBlock(square, !square->roadBlock);
            }
            else if (photo) {
                for (auto & i : map->stopped) {
                    if (i.photo == photo) {
                        i = BusStopRoadBlock(i);
                    }
                }
            }
            else {
                std::cerr << "Warning: off-road click\n";
            }
        }
        else {
            /* tooltip of bus is number, size is 1 */
            if (photo && photo->toolTip().size() == 1) {
                ShowRoute(photo);
            }
        }
    }
}

void
Scene::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton) {
        m_moving = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void
Scene::mouseMoveEvent(QMouseEvent *event)
{
    if (m_moving) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_originalX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_originalY));
        m_originalX = event->x();
        m_originalY = event->y();
        event->accept();
        return;
    }
    event->ignore();
}

void
Scene::ZoomAdd()
{
    double scaleFactor = 1.15;
    scale(scaleFactor,scaleFactor);
    zoom_act *= scaleFactor;
    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void
Scene::ZoomSub()
{
    double scaleFactor = 1.15;
    scale(1/scaleFactor,1/scaleFactor);
    zoom_act = zoom_act*(1/scaleFactor);
    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void
Scene::MoveBuses()
{
    garage.MoveAllBuses(map, scene);
}

void
Scene::StreetUpdate(float updateSlowdown, const std::string& name)
{
    map->UpdateStreet(name, updateSlowdown);
}

void
Scene::SquareRoadBlock(Square* square, bool onOff)
{
    if (!square || !square->road)
        return;

    int x = square->row;
    int y = square->col;
    int startInt,endInt;
    Coordinates *start,*end;
    QMessageBox Msgbox;
    bool xy;
    Msgbox.setText("There is a bus on the route, the street cannot be closed!");

    if (x < 0 or x > 138)
        return;

    if (y < 0 or y > 79)
        return;

    if (x + 1 >= 0 and x + 1 < 138)
        startInt = x + 1;
    else
        startInt = x - 1;

    /* distance between stops x axis */
    if (Square::layout[startInt][y]->road) {
        startInt = x;
        endInt = x;
        xy = true;
        for (; !Square::layout[endInt + 1][y]->hasStop; endInt++) {
        }
        for (; !Square::layout[startInt - 1][y]->hasStop; startInt--) {
        }

        start = new Coordinates(startInt, y);
        end = new Coordinates(endInt, y);
    }
    /* y axis */
    else {
        startInt = y;
        endInt = y;
        xy = false;
        for (; !Square::layout[x][endInt + 1]->hasStop; endInt++) {
        }
        for (; !Square::layout[x][startInt - 1]->hasStop; startInt--) {
        }
        start = new Coordinates(x, startInt);
        end = new Coordinates(x, endInt);

    }

    /* Check if there is a bus on the desired street that wants to close */
    for (auto *bus : garage.allBuses) {
        if (bus->busPosition->x == x) {
            if (end->y >= bus->busPosition->y && bus->busPosition->y >= start->y) {
                Msgbox.exec();
                return;
            }
        }
        else if (bus->busPosition->y == y) {
            if (end->x >= bus->busPosition->x && bus->busPosition->x >= start->x) {
                Msgbox.exec();
                return;
            }
        }
    }

    Square *squareX, *squareY;
    /* Add RoadStop */
    for (; startInt <= endInt; startInt++) {
        squareX = Square::layout[x][startInt];
        squareY = Square::layout[startInt][y];

        if (onOff) {
            if (xy) {
                squareY->roadBlock = true;
                squareY->SetColor("#FF0000");
            }
            else {
                squareX->roadBlock = true;
                squareX->SetColor("#FF0000");
            }
        }
        else {
            if (xy) {
                squareY->roadBlock = false;
                squareY->SetColor("#C0C0C0");
            }
            else {
                squareX->roadBlock = false;
                squareX->SetColor("#C0C0C0");
            }
        }
    }
}

StreetMap::stopData
Scene::BusStopRoadBlock(StreetMap::stopData stop)
{
    QString path;
    QMessageBox Msgbox;
    Msgbox.setText("On the road is a bus, the stop cannot be closed");

    if (!stop.stop->roadStop) {
        for (auto *bus : garage.allBuses) {
            if (bus->nextBusStop.name == stop.stop->stopName ) {
                Msgbox.exec();
                return stop;
            }
        }
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop_roadblock.jpg"));
        Square::layout[stop.stop->coordinates->x][stop.stop->coordinates->y]->roadBlock = true;
    }
    else {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));
        Square::layout[stop.stop->coordinates->x][stop.stop->coordinates->y]->roadBlock = false;
    }

    stop.photo = stop.stop->AddStopToScene(scene, path);

    return stop;
}

void
Scene::ShowRoute(QGraphicsItem *photo)
{
    static std::vector<QGraphicsItem*> busPhotoStorage;
    static Bus *bus;
    QString colors[4] = {"", "#ff4040", "#75a298", "#daccc4"};
    std::string routeColor = "#c0c0c0";

    /* find photo in storage */
    auto end = std::end(busPhotoStorage);
    auto found = std::find(std::begin(busPhotoStorage), end, photo);
    bus = garage.GetBusByPhoto(photo);

    if (found == end) {
        if (!bus) {
            std::cerr << "Error: Bus not found\n";
        }
        else {
            busPhotoStorage.push_back(photo);
            bus->CreateTimetable(colors[bus->busNumber_]);
        }
    } /* second click on same bus changes route to default color */
    else {
        BusRouteMap::DrawLine(bus->stopInformation, QString::fromStdString(routeColor));
        bus->ClearTextArea();
        busPhotoStorage.erase(found);
    }
}

void
Scene::CheckRoadBlockBus()
{
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int secNow = Timer::GetSecond();
    int i = 0;
    Coordinates *busCoordinates;

    for (auto *bus : garage.allBuses) {
        int nxt, nw, mn;
        for (; i < bus->stopInformation.size() - 2; i++) {
            nxt = bus->stopInformation[i + 1].stopHour * 60 + bus->stopInformation[i + 1].stopMin;
            nw  = hourNow * 60 + minuteNow;
            mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

            if (nxt > nw and nw >= mn) {
                i++;
                break;
            }
        }

        for (; i < bus->stopInformation.size() - 2; i++) {
            busCoordinates = bus->stopInformation[i].coordinates;
            if (Square::layout[busCoordinates->x][busCoordinates->y]->roadBlock) {
                /* roadStop on road */
                //std::cerr << "Stop Closed " << bus->stopInformation[i].name << "\n";
                bus->roadStopOnRoad = true;
            }
            /* street going vertically */
            if (bus->stopInformation[i].coordinates->x == bus->stopInformation[i + 1].coordinates->x ) {
                if (Square::layout[bus->stopInformation[i].coordinates->x][(bus->stopInformation[i].coordinates->y + bus->stopInformation[i + 1].coordinates->y) / 2 ]->roadBlock) {
                    //std::cerr << "RoadStop between " << bus->stopInformation[i].name << " a " << bus->stopInformation[i+1].name << "\n";
                    /* roadStop */
                    bus->roadStopOnRoad = true;
                }
            }
        }
    }
}
