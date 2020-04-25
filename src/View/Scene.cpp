#include "Scene.h"

Scene::Scene(QWidget *parent) : QGraphicsView(parent)
{
    graphicsScene = new QGraphicsScene(parent);
    SetUpView();
    CreateMap();
}

Scene::~Scene()
{
    /* delete all buses and map */
    garage.DeleteBuses(graphicsScene);
    for (auto list : StreetMap::stopList) {
        delete list.stop;
        delete list.photo;
    }
    std::vector<StreetMap::stopData>().swap(StreetMap::stopList);
    delete map;
    delete markedBus;
}

void
Scene::CreateMap()
{
    Timer::GetTime();
    /* initialize map */
    map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets(Functions::GetAbsolutePath("../examples/ulice.txt"));
    map->AddStops(Functions::GetAbsolutePath("../examples/zastavky.txt"), graphicsScene);
    /* add squares to scene */
    AddSquares();
    /* add buses */
    AddBuses();
}

void
Scene::AddBuses() {
    int iteration = 0;

    if (busId >= 3) {
        iteration = garage.allBuses[garage.allBuses.size() - 1]->iteration + 1;
    }

    garage.AddBus(busId++, 1, graphicsScene, iteration);
    garage.AddBus(busId++, 2, graphicsScene, iteration);
    garage.AddBus(busId++, 3, graphicsScene, iteration);

    if (busId == 3) {
        /* add the number of buses to the scene according to current time */
        AddBusOneByOne();
        return;
    }
}

void
Scene::AddBusOneByOne()
{
    static int route = 3;
    static int busNumber = 1;
    static int originalBusId = busId;

    if (!route && busNumber > 3) {
        route = 3;
        busNumber = 1;
    }

    Bus *found = garage.GetBus(originalBusId - route);
    if (found) {
        int minuteNow = Timer::GetMinute();
        int stopMin = found->stopInformation[0].stopMin;

        if (minuteNow >= stopMin + 10) {
            minuteNow /= 10;
            for (int i = 0; i < minuteNow; i++) {
                garage.AddBus(busId, busNumber, graphicsScene, i + 1);
                busId++;
            }
        }
    }

    route--;
    busNumber++;
    IF(route, AddBusOneByOne())
}

void
Scene::AddSquares()
{
    for (auto &x : Square::layout) {
        for (auto &y : x) {
            graphicsScene->addItem(y);
        }
    }
}

void
Scene::SetUpView()
{
    setFixedSize(X * SQUARE_SIZE, Y * SQUARE_SIZE);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(graphicsScene);
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
            if (markedBus != nullptr) {
                QMessageBox Msgbox;
                int i = 0;

                if (markedBus->newstopInformation.empty())
                {
                    for(; markedBus->stopInformation[i].name != markedBus->nextBusStop.name; i++) {
                        markedBus->newstopInformation.push_back(markedBus->stopInformation[i]);
                    }
                    markedBus->newstopInformation.push_back(markedBus->nextBusStop);
                }

                if (square) {
                    if (square->roadBlock) {
                        Msgbox.setText("Not this way, the road is closed");
                        Msgbox.exec();
                    }
                    if (square->GetColor() != "#331331") {
                        bool xy;
                        int startInt,endInt;
                        Coordinates::Coordinates_S start, end;

                        //GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

                        /* y axis */
                        if (start.x == end.x) {
                            if (Square::layout[start.x][start.y - 1]->road) {

                                std::string stopFirstName = StreetMap::GetStopByCoordinates(start.x, start.y - 1);
                                std::string stopSecondtName = StreetMap::GetStopByCoordinates(start.x, end.y + 1);
                                Coordinates::BusStop_S information;

                                if (markedBus->newstopInformation[i-1].name == stopFirstName) {
                                    information.coordinates.x = start.x;
                                    information.coordinates.y = end.y + 1;
                                    information.name = stopSecondtName;
                                }

                                if( end.y - start.y == 10) {
                                        if (information.stopMin + 2 > 60 ) {
                                            information.stopHour = markedBus->newstopInformation[i-1].stopHour + 1;
                                            information.stopMin = markedBus->newstopInformation[i-1].stopMin + 2 - 60;
                                        }
                                        else {
                                            information.stopHour = markedBus->newstopInformation[i-1].stopHour;
                                            information.stopMin = markedBus->newstopInformation[i-1].stopMin + 2;
                                        }
                                }
                                else {
                                    if (information.stopMin + 3 > 60 ) {
                                        information.stopHour = markedBus->newstopInformation[i-1].stopHour + 1;
                                        information.stopMin = markedBus->newstopInformation[i-1].stopMin + 3 - 60;
                                    }
                                    else {
                                        information.stopHour = markedBus->newstopInformation[i-1].stopHour;
                                        information.stopMin = markedBus->newstopInformation[i-1].stopMin + 3;
                                    }
                                }
                            }
                        }
                    }
                    else {
                        Square::layout[square->row][square->col]->SetColor("#c0c0c0");
                        // oddělat zastávku
                    }


                }
                else if (photo) {
                    for (auto &list : StreetMap::stopList) {
                        if (list.photo == photo) {

                        }
                    }
                    for (auto &b : garage.allBuses) {
                        if (b->busPhoto == photo) {
                            markedBus = nullptr;
                        }
                    }
                }
            }
            else if (square) {
                SquareRoadBlock(square, !square->roadBlock);
            }
            else if (photo) {
                for (auto &i : StreetMap::stopList) {
                    if (i.photo == photo) {
                        i = BusStopRoadBlock(i);
                    }
                }
                for (auto &b : garage.allBuses) {
                    if (b->busPhoto == photo) {
                            markedBus = b;
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
Scene::mouseReleaseEvent(QMouseEvent *event)
{
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
    garage.MoveAllBuses(map, graphicsScene);
}

void
Scene::StreetUpdate(float updateSlowdown, const std::string& name)
{
    map->UpdateStreet(name, updateSlowdown);
}

void
Scene::GetStreetPositionBetweenStops (Square* square, Coordinates **start, Coordinates **end, 
                                      int* startInt, int* endInt, bool *xy) 
{
//    int x = square->row;
//    int y = square->col;
//    int s,e;
//    int startInt,endInt;
//    Coordinates *start,*end;
//    QMessageBox Msgbox;
//    bool xy;
//    Msgbox.setText("There is a bus on the route, the street cannot be closed!");
//
//    if (x < 0 or x > 138)
//        return;
//
//    if (y < 0 or y > 79)
//        return;
//
//    s = x + 1 >= 0 and x + 1 < 138 ? x + 1 : x - 1;
//
//    /* distance between stops x axis */
//    if (Square::layout[s][y]->road) {
//        s = x;
//        e = x;
//        *xy = true;
//        for (; !Square::layout[e + 1][y]->hasStop; e++) {
//        }
//        for (; !Square::layout[s - 1][y]->hasStop; s--) {
//        }
//
//        *startInt = s;
//        *endInt = e;
//        *start = new Coordinates(s, y);
//        *end = new Coordinates(e, y);
//        startInt = x + 1;
//    }
//    else {
//        startInt = x - 1;
//    }
//
//    /* distance between stops x axis */
//    if (Square::layout[startInt][y]->road) {
//        startInt = x;
//        endInt = x;
//        xy = true;
//
//        for (; !Square::layout[endInt + 1][y]->hasStop; endInt++) {
//        }
//        for (; !Square::layout[startInt - 1][y]->hasStop; startInt--) {
//        }
//
//        start = new Coordinates(startInt, y);
//        end = new Coordinates(endInt, y);
//    }
//        /* y axis */
//    else {
//        s = y;
//        e = y;
//        *xy = false;
//        for (; !Square::layout[x][e + 1]->hasStop; e++) {
//        }
//        for (; !Square::layout[x][s - 1]->hasStop; s--) {
//        }
//        *startInt = s;
//        *endInt = e;
//        *start = new Coordinates(x, s);
//        *end = new Coordinates(x, e);
//        startInt = y;
//        endInt = y;
//        xy = false;
//
//        for (; !Square::layout[x][endInt + 1]->hasStop; endInt++) {
//        }
//        for (; !Square::layout[x][startInt - 1]->hasStop; startInt--) {
//        }
//
//        start = new Coordinates(x, startInt);
//        end = new Coordinates(x, endInt);
//    }
}

void
Scene::SquareRoadBlock(Square* square, bool onOff)
{
    if (!square || !square->road)
        return;

    bool xy;
    QMessageBox Msgbox;
    Msgbox.setText("There is a bus on the route, the street cannot be closed!");


    int x = square->row;
    int y = square->col;
    int startInt,endInt;
    Coordinates::Coordinates_S start, end;

    //GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

    /* Check if there is a bus on the desired street that wants to close */
    for (auto *bus : garage.allBuses) {
        if (bus->busPosition.x == x) {
            if (end.y >= bus->busPosition.y && bus->busPosition.y >= start.y) {
                Msgbox.exec();
                return;
            }
        }
        else if (bus->busPosition.y == y) {
            if (end.x >= bus->busPosition.x && bus->busPosition.x >= start.x) {
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
        Square::layout[stop.stop->coordinates.x][stop.stop->coordinates.y]->roadBlock = true;
    }
    else {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));
        Square::layout[stop.stop->coordinates.x][stop.stop->coordinates.y]->roadBlock = false;
    }

    stop.photo = stop.stop->AddStopToScene(graphicsScene, path);

    return stop;
}

void
Scene::InitTimetableArea(QWidget *parent, int width, int height)
{
    /* text area for bus timetable */
    textArea = new QPlainTextEdit(parent);
    textArea->setMinimumSize(width * 0.19, height * 0.6);
    textArea->move(5, 115);
}

void
Scene::ShowRoute(QGraphicsItem *photo)
{
    static Bus *seenBus = nullptr;
    Bus *bus;
    QString colors[4] = {"", "#9f92ca", "#f46c6e", "#77bcbd"};
    QString routeColor = "#c0c0c0";

    bus = garage.GetBusByPhoto(photo);
    if (!seenBus || seenBus != bus) {
        if (seenBus && seenBus != bus) {
            BusRouteMap::DrawLine(seenBus->stopInformation, routeColor);
            textArea->clear();
        }

        if (!bus) {
            std::cerr << "Error: Bus not found\n";
        }
        else {
            seenBus = bus;
            bus->CreateTimetable(colors[bus->busNumber_], textArea);
        }
    } /* second click on same bus changes route to default color */
    else {
        BusRouteMap::DrawLine(bus->stopInformation, routeColor);
        textArea->clear();
        seenBus = nullptr;
    }
}

void
Scene::CheckRoadBlockBus()
{
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int i = 0;
    int stopInformationSize;

    for (auto *bus : garage.allBuses) {
        int nxt, nw, mn;
        stopInformationSize = (int)bus->stopInformation.size();
        for (; i < stopInformationSize - 2; i++) {
            nxt = bus->stopInformation[i + 1].stopHour * 60 + bus->stopInformation[i + 1].stopMin;
            nw  = hourNow * 60 + minuteNow;
            mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

            if (nxt > nw and nw >= mn) {
                i++;
                break;
            }
        }

        for (; i < stopInformationSize - 2; i++) {
            if (Square::layout[bus->stopInformation[i].coordinates.x][bus->stopInformation[i].coordinates.y]->roadBlock) {
                /* roadStop on road */
                //std::cerr << "Stop Closed " << bus->stopInformation[i].name << "\n";
                bus->roadStopOnRoad = true;
            }
            /* street going vertically */
            if (bus->stopInformation[i].coordinates.x == bus->stopInformation[i + 1].coordinates.x ) {
                if (Square::layout[bus->stopInformation[i].coordinates.x][(bus->stopInformation[i].coordinates.y + bus->stopInformation[i + 1].coordinates.y) / 2 ]->roadBlock) {
                    //std::cerr << "RoadStop between " << bus->stopInformation[i].name << " a " << bus->stopInformation[i+1].name << "\n";
                    /* roadStop */
                    bus->roadStopOnRoad = true;
                }
            }
        }
    }
}
