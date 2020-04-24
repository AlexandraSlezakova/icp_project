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
    garage.AddBus(busId++, 1, scene, iteration);
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
                        Coordinates *start, *end;

                        GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

                        /* y axis */
                        if (start->x == end->x) {
                            if (Square::layout[start->x][start->y - 1]->road) {

                                std::string stopFirstName = Stop::GetStopByCoordinates(start->x, start->y - 1);
                                std::string stopSecondtName = Stop::GetStopByCoordinates(start->x, end->y + 1);
                                Coordinates::BusStop_S information;

                                if (markedBus->newstopInformation[i-1].name == stopFirstName) {

                                    information.coordinates = new Coordinates(start->x, end->y + 1);
                                    information.name = stopSecondtName;
                                }

                                if( end->y - start->y == 10) {
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
                    for (auto &i : map->stopped) {
                        if (i.photo == photo) {

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
                for (auto &i : map->stopped) {
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
Scene::GetStreetPositionBetweenStops (Square* square, Coordinates* *start, Coordinates* *end, int* startInt, int* endInt, bool *xy) {

    int x = square->row;
    int y = square->col;
    int s,e;

    if (x < 0 or x > 138)
        return;

    if (y < 0 or y > 79)
        return;

    if (x + 1 >= 0 and x + 1 < 138)
        s = x + 1;
    else
        s = x - 1;

    /* distance between stops x axis */
    if (Square::layout[s][y]->road) {
        s = x;
        e = x;
        *xy = true;
        for (; !Square::layout[e + 1][y]->hasStop; e++) {
        }
        for (; !Square::layout[s - 1][y]->hasStop; s--) {
        }

        *startInt = s;
        *endInt = e;
        *start = new Coordinates(s, y);
        *end = new Coordinates(e, y);
    }
        /* y axis */
    else {
        s = y;
        e = y;
        *xy = false;
        for (; !Square::layout[x][e + 1]->hasStop; e++) {
        }
        for (; !Square::layout[x][s - 1]->hasStop; s--) {
        }
        *startInt = s;
        *endInt = e;
        *start = new Coordinates(x, s);
        *end = new Coordinates(x, e);
    }
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
    Coordinates *start, *end;

    GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

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

    /* Add RoadStop */
    for (; startInt <= endInt; startInt++) {
        if (onOff) {
            if (xy) {
                Square::layout[startInt][y]->roadBlock = true;
                Square::layout[startInt][y]->SetColor("#FF0000");
            }
            else {
                Square::layout[x][startInt]->roadBlock = true;
                Square::layout[x][startInt]->SetColor("#FF0000");
            }
        }
        else {
            if (xy) {
                Square::layout[startInt][y]->roadBlock = false;
                Square::layout[startInt][y]->SetColor("#C0C0C0");
            }
            else {
                Square::layout[x][startInt]->roadBlock = false;
                Square::layout[x][startInt]->SetColor("#C0C0C0");
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

Bus *Scene::NewBusRoad(Bus *Bus) {
    return nullptr;
}
