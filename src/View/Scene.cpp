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
    for(int i = 1; i <= 6; i++) {
        garage.AddBus(busId++, 1, graphicsScene, 6 - i);
    }
    for(int i = 1; i <= 3; i++) {
        garage.AddBus(busId++, 2, graphicsScene, 3 - i);
    }
    for(int i = 1; i <= 3; i++) {
        garage.AddBus(busId++, 3, graphicsScene, 3 - i);
    }
}

void
Scene::AddBusOneByOne()
{
    int busLine1 = 0;
    int busLine2 = 0;
    int busLine3 = 0;

    for (auto *bus:garage.allBuses) {
        if (bus->busNumber_ == 1) {
            busLine1++;
        }
        else if (bus->busNumber_ == 2) {
            busLine2++;
        }
        else {
            busLine3++;
        }
    }

    for(; busLine1 <= 6; busLine1++) {
        garage.AddBus(busId++, 1, graphicsScene, 6 - busLine1);
    }
    for(; busLine2 <= 3; busLine2++) {
        garage.AddBus(busId++, 2, graphicsScene, 3 - busLine2);
    }
    for(; busLine3 <= 3; busLine3++) {
        garage.AddBus(busId++, 3, graphicsScene, 3 - busLine3);
    }
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
                RoadStopBusService(square,photo);
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
                for (auto *b : garage.allBuses) {
                    if (b->busPhoto == photo) {
                        BusPick(b);
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
Scene::BusPick(Bus *b) {
    markedBus = b;
    markedBus->newStopInformation.clear();
    MapClean();
    QString colors[4] = {"#ff00aa", "#9f92ca", "#f46c6e", "#77bcbd"};
    if (markedBus->newStopInformation.empty()) {
        unsigned int i = 0;
        /* saves all stops that the bus has already passed */
        for (; markedBus->stopInformation[i].name != markedBus->nextBusStop.name; i++) {
            markedBus->newStopInformation.push_back(markedBus->stopInformation[i]);
        }
        if ( Square::layout[(markedBus->currentBusStop.coordinates.x + markedBus->nextBusStop.coordinates.x) / 2]
        [(markedBus->currentBusStop.coordinates.y + markedBus->nextBusStop.coordinates.y) / 2]->GetColor() != "#ff0000") {
            markedBus->newStopInformation.push_back(markedBus->nextBusStop);
        }
        else
            i--;

        unsigned int k = 0;
        /* plotting all stops that the bus has already passed */
        for (; k < i; k++) {
            Coordinates::BusStop_S first = markedBus->stopInformation[k];
            Coordinates::BusStop_S second = markedBus->stopInformation[k + 1];
            PlottingRouteBetweenStops(first, second, colors[0]);
        }

        /* plotting the rest of the path for possible binding */
        for (; k < markedBus->stopInformation.size() - 1; k++) {
            Coordinates::BusStop_S first = markedBus->stopInformation[k];
            Coordinates::BusStop_S second = markedBus->stopInformation[k + 1];
            PlottingRouteBetweenStops(first, second, colors[markedBus->busNumber_]);
        }
        markedBus->stopInformation.swap(markedBus->newStopInformation);
    }
}

void
Scene::PlottingRouteBetweenStops(const Coordinates::BusStop_S &first, const Coordinates::BusStop_S &second,QString colour) const {
    int startInt, endInt;
    if ( first.coordinates.x == second.coordinates.x ) {
        startInt = first.coordinates.y;
        endInt = second.coordinates.y;
        if (startInt > endInt) {
            std::swap(startInt,endInt);
            startInt++;
        }
        for (; startInt < endInt; startInt++) {
            if (Square::layout[first.coordinates.x][startInt]->GetColor() != "#ff0000")
                Square::layout[first.coordinates.x][startInt]->SetColor(colour);
        }
    }
    else {
        startInt = first.coordinates.x;
        endInt = second.coordinates.x;
        if (startInt > endInt) {
            std::swap(startInt,endInt);
            startInt++;
        }
        for (; startInt < endInt; startInt++) {
            if (Square::layout[startInt][first.coordinates.x]->GetColor() != "#ff0000")
                Square::layout[startInt][first.coordinates.y]->SetColor(colour);
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
Scene::GetStreetPositionBetweenStops(Square* square, Coordinates::Coordinates_S *start, Coordinates::Coordinates_S *end,
                                      int* startInt, int* endInt, bool *xy) 
{
    int x = square->row;
    int y = square->col;
    int s,e;

    if (x < 0 or x > 138)
        return;

    if (y < 0 or y > 79)
        return;

    s = x + 1 >= 0 and x + 1 < 138 ? x + 1 : x - 1;

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
        start->x = s;
        start->y = y;
        end->x = e;
        end->y = y;
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
        start->x = x;
        start->y = s;
        end->x = x;
        end->y = e;
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
    Coordinates::Coordinates_S start, end;

    GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

    /* Check if there is a bus on the desired street that wants to close */
    for (auto *bus : garage.allBuses) {
        if (startInt > endInt) {
            std::swap(startInt,endInt);
            startInt++;
            endInt --;
        }
        if (bus->busPosition.x == x) {
            if (endInt >= bus->busPosition.y && bus->busPosition.y >= startInt) {
                Msgbox.exec();
                return;
            }
        }
        else if (bus->busPosition.y == y) {
            if (startInt > endInt) {
                std::swap(startInt,endInt);
                startInt++;
                endInt--;
            }
            if (endInt >= bus->busPosition.x && bus->busPosition.x >= startInt) {
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

    for (auto *bus : garage.allBuses) {
        garage.CheckRoadBlockLongDistance(bus);
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
        stop.stop->roadStop = true;

    }
    else {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));
        Square::layout[stop.stop->coordinates.x][stop.stop->coordinates.y]->roadBlock = false;
        stop.stop->roadStop = false;
    }

    stop.photo = stop.stop->AddStopToScene(graphicsScene, path);

    return stop;
}

void
Scene::MapClean() {
    for (auto & x : Square::layout) {
        for (int y = 0; y < Y; y++) {
            if (x[y]->road) {
                if (x[y]->roadBlock) {
                    x[y]->SetColor("#ff0000");
                }
                else {
                    x[y]->SetColor("#c0c0c0");
                }
            }
        }
    }
    map->UpdateAllStreet();
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
    Bus *bus;
    QString colors[4] = {"", "#9f92ca", "#f46c6e", "#77bcbd"};
    QString routeColor = "#c0c0c0";

    bus = garage.GetBusByPhoto(photo);
    if (!seenBus || seenBus != bus) {
        if (seenBus && seenBus != bus) {
            BusRouteMap::DrawLine(seenBus->stopInformation, routeColor);
            textArea->clear();
            seenBus->textArea = nullptr;
        }

        if (!bus) {
            std::cerr << "Error: Bus not found\n";
        }
        else {
            seenBus = bus;
            bus->CreateTimetable(colors[bus->busNumber_], textArea);
            bus->textArea = textArea;
        }
    } /* second click on same bus changes route to default color */
    else {
        bus->textArea = nullptr;
        textArea->clear();
        seenBus = nullptr;
        MapClean();
    }
}



void
Scene::RoadStopBusService (Square *square, QGraphicsPixmapItem *photo) {

    QMessageBox Msgbox;
    int i = markedBus->stopInformation.size() - 1;
    std::string color;

    if (square) {
        if (square->roadBlock) {
            Msgbox.setText("Not this way, the road is closed");
            Msgbox.exec();
        }
        if (square->GetColor() == "#c0c0c0" || square->GetColor() == "#9f92ca"
            || square->GetColor() =="#f46c6e"|| square->GetColor() == "#77bcbd") {
            bool xy;
            int startInt,endInt;
            Coordinates::Coordinates_S start, end;

            GetStreetPositionBetweenStops(square, &start, &end, &startInt, &endInt, &xy);

            std::string stopFirstName, stopSecondName;

            Coordinates::BusStop_S information;

            /* y axis */
            if (start.x == end.x) {

                stopFirstName = StreetMap::GetStopByCoordinates(start.x, start.y - 1);
                stopSecondName = StreetMap::GetStopByCoordinates(start.x, end.y + 1);

                /* adding new stop */
                if (markedBus->stopInformation[i].name == stopFirstName) {
                    information.coordinates.x = start.x;
                    information.coordinates.y = end.y + 1;
                    information.name = stopSecondName;
                }
                else if (markedBus->stopInformation[i].name == stopSecondName) {
                    information.coordinates.x = start.x;
                    information.coordinates.y = start.y - 1;
                    information.name = stopFirstName;
                }
                else {
                    Msgbox.setText("It's not a close way");
                    Msgbox.exec();
                    return;
                }
            }
            else if (start.y == end.y) {
                stopFirstName = StreetMap::GetStopByCoordinates(start.x - 1, start.y);
                stopSecondName = StreetMap::GetStopByCoordinates(end.x + 1, start.y);


                if (markedBus->stopInformation[i].name == stopFirstName) {
                    information.coordinates = StreetMap::GetStopByName(stopSecondName);
                    information.name = stopSecondName;
                }
                else if (markedBus->stopInformation[i].name == stopSecondName) {
                    information.coordinates = StreetMap::GetStopByName(stopFirstName);
                    information.name = stopFirstName;
                }
                else {
                    Msgbox.setText("It's not a close way");
                    Msgbox.exec();
                    return;
                }
            }

            /* cout arriving time on stop  */
            if (abs (markedBus->stopInformation[i].coordinates.x + markedBus->stopInformation[i].coordinates.y -
            information.coordinates.x - information.coordinates.y) == 10) {
                if (information.stopMin + 2 >= 60 ) {
                    information.stopHour = markedBus->stopInformation[i].stopHour + 1;
                    information.stopMin = markedBus->stopInformation[i].stopMin + 2 - 60;
                }
                else {
                    information.stopHour = markedBus->stopInformation[i].stopHour;
                    information.stopMin = markedBus->stopInformation[i].stopMin + 2;
                }
            }
            else {
                if (information.stopMin + 3 >= 60 ) {
                    information.stopHour = markedBus->stopInformation[i].stopHour + 1;
                    information.stopMin = markedBus->stopInformation[i].stopMin + 3 - 60;
                }
                else {
                    information.stopHour = markedBus->stopInformation[i].stopHour;
                    information.stopMin = markedBus->stopInformation[i].stopMin + 3;
                }
            }
            /* save stop */
            markedBus->stopInformation.push_back(information);
            i++;

            if (markedBus->stopMoving) {
                markedBus->GetBusStops();
            }

            Coordinates::BusStop_S first = markedBus->stopInformation[i - 1];
            Coordinates::BusStop_S second = markedBus->stopInformation[i];

            /* printing new add stop and road to stop */
            if ( first.coordinates.x == second.coordinates.x ) {
                for (; startInt <= endInt; startInt++) {
                    Square::layout[first.coordinates.x][startInt]->SetColor("#ff00aa");
                }
            }
            else {
                for (; startInt <= endInt; startInt++) {
                    Square::layout[startInt][first.coordinates.y]->SetColor("#ff00aa");
                }
            }

            for (auto & stop : markedBus->newStopInformation) {
                if (information.name == stop.name) {
                    ContinueBusRoute(information);
                }
            }
        }
        else if (square->GetColor() == "#ff00aa") {
            /* deleting new add stops and way */
            Msgbox.setText("Cannot delete, not the last path");
            Coordinates::BusStop_S first = markedBus->stopInformation[i - 1];
            Coordinates::BusStop_S second = markedBus->stopInformation[i];
            int x = square->row;
            int y = square->col;
            int start, end;



            if (first.coordinates.x == second.coordinates.x && second.coordinates.x == x) {
                if (first.coordinates.y < y && y < second.coordinates.y) {
                    start = first.coordinates.y;
                    end = second.coordinates.y;
                }
                else if(first.coordinates.y > y && y > second.coordinates.y) {
                    start = second.coordinates.y;
                    end = first.coordinates.y;
                }
                else {
                    Msgbox.exec();
                    return;
                }

                if (end > markedBus->busPosition.y &&
                markedBus->busPosition.y > start && markedBus->busPosition.x == x) {
                    Msgbox.setText("Cannot delete route, the bus is on it! \n If the bus is in a dead end street, "
                                   "click on the stops to get back");
                    Msgbox.exec();
                    return;
                }

                for (; start <= end; start++) {
                    Square::layout[first.coordinates.x][start]->SetColor("#c0c0c0");
                }
            }
            else if (first.coordinates.y == second.coordinates.y && second.coordinates.y == y) {
                if (first.coordinates.x < x && x < second.coordinates.x) {
                    start = first.coordinates.x;
                    end = second.coordinates.x;
                }
                else if(first.coordinates.x > x && x > second.coordinates.x) {
                    start = second.coordinates.x;
                    end = first.coordinates.x;
                }
                else {
                    Msgbox.exec();
                    return;
                }

                if (end > markedBus->busPosition.x && markedBus->busPosition.x > start
                && markedBus->busPosition.y == y) {
                    Msgbox.setText("Cannot delete route, the bus is on it! \n If the bus is in a dead end street, "
                                   "click on the stops to get back");
                    Msgbox.exec();
                    return;
                }

                for (; start <= end; start++) {
                    Square::layout[start][first.coordinates.y]->SetColor("#c0c0c0");
                }
            }
            else {
                Msgbox.exec();
                return;
            }
            markedBus->stopInformation.pop_back();
        }
    }
    else if (photo) {
        /* repick marked bus or click on stop and add to bus route or remove   */
        for (auto &list : StreetMap::stopList) {
            if (list.photo == photo) {
                Coordinates::BusStop_S first = markedBus->stopInformation[markedBus->stopInformation.size()-1];
                Msgbox.setText("This path cannot be selected, there is a roadblock on the road!");
                Coordinates::BusStop_S information;

                int squareCount = abs(first.coordinates.x + first.coordinates.y
                        - list.coordinates.x - list.coordinates.y);

                if (squareCount == 10) {
                    if (Square::layout[(first.coordinates.x + list.coordinates.x)/2][(first.coordinates.y
                    + list.coordinates.y)/2]->roadBlock) {
                        Msgbox.exec();
                        return;
                    }
                    information.coordinates = list.coordinates;
                    information.name = list.stop->stopName;

                    if (first.stopMin + 2 > 60) {
                        information.stopHour = first.stopHour + 1;
                        information.stopMin = first.stopMin + 2 - 60;
                    }
                    else {
                        information.stopHour = first.stopHour;
                        information.stopMin = first.stopMin + 2;
                    }
                }
                else if (squareCount == 19) {

                    if (Square::layout[(first.coordinates.x + list.coordinates.x)/2][(first.coordinates.y
                    + list.coordinates.y)/2]->roadBlock) {
                        Msgbox.exec();
                        return;
                    }

                    information.coordinates = list.coordinates;
                    information.name = list.stop->stopName;

                    if (first.stopMin + 3 > 60) {
                        information.stopHour = first.stopHour + 1;
                        information.stopMin = first.stopMin + 3 - 60;
                    }
                    else {
                        information.stopHour = first.stopHour;
                        information.stopMin = first.stopMin + 3;
                    }
                }
                else {
                    Msgbox.setText("The stop is not located near the last bus stop!");
                    Msgbox.exec();
                    return;
                }

                markedBus->stopInformation.push_back(information);

                PlottingRouteBetweenStops(first,information,"#ff00aa");

                ContinueBusRoute(information);
            }
        }
        if (markedBus->busPhoto == photo) {
            MapClean();
            markedBus = nullptr;
            return;
        }
        for (auto &b : garage.allBuses) {
            if (b->busPhoto == photo) {
                BusPick(b);
            }
        }
    }
}

void
Scene::ContinueBusRoute(Coordinates::BusStop_S &information) {
    for (unsigned int c = 0; c < markedBus->newStopInformation.size(); c++) {
        if (information.name == markedBus->newStopInformation[c].name) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "RoadStop", "continue on the bus line?",
                    QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                c++;
                for (; c < markedBus->newStopInformation.size(); ++c) {
                    Coordinates::BusStop_S sIStop = markedBus->stopInformation[markedBus->stopInformation.size() - 1];
                    Coordinates::BusStop_S nIStop = markedBus->newStopInformation[c];
                    if ( abs(sIStop.coordinates.x + sIStop.coordinates.y
                    - nIStop.coordinates.x - nIStop.coordinates.y) == 10) {
                        if (sIStop.stopMin + 2 > 60) {
                            nIStop.stopHour = sIStop.stopHour + 1;
                            nIStop.stopMin = sIStop.stopMin + 2 - 60;
                        }
                        else {
                            nIStop.stopMin = sIStop.stopMin + 2;
                            nIStop.stopHour = sIStop.stopHour;
                        }
                    }
                    else {
                        if (sIStop.stopMin + 3 > 60) {
                            nIStop.stopHour = sIStop.stopHour + 1;
                            nIStop.stopMin = sIStop.stopMin + 3 - 60;
                        }
                        else {
                            nIStop.stopMin = sIStop.stopMin + 3;
                            nIStop.stopHour = sIStop.stopHour;
                        }
                    }
                    markedBus->stopInformation.push_back(nIStop);
                    PlottingRouteBetweenStops(markedBus->stopInformation[markedBus->stopInformation.size() - 2],
                                              markedBus->stopInformation[markedBus->stopInformation.size() - 1],
                                              "#ff00aa");
                }
            }
        }
    }
}
