#include <unistd.h>
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
    /* add it to scene */
    AddMap(map);
    /* add buses */
    garage = new Garage(busId,1,scene);
    MoveBuses();
}

void
Scene::AddMap(StreetMap *streetMap)
{
    for (auto & x : streetMap->layout) {
        for (auto & y : x) {
            scene->addItem(y);
        }
    }
}

void
Scene::SetUpView()
{
    //setFixedSize(X * SQUARE_SIZE, Y * SQUARE_SIZE);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
}

void
Scene::GetBus1Timetable()
{
    Bus* bus = garage->GetBus(0, 1);
    bus->CreateTimetable(text, map->layout, "#FF0000");
    bus->MoveBus();
}

void
Scene::MoveBuses()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(MoveBus()));
    /* time in ms */
    timer->start(1000);
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

void Scene::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton) {
        m_moving = true;
        m_originalX = event->x();
        m_originalY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    else if (event->button() == Qt::LeftButton && roadBlockMode) {
        QGraphicsItem *item = itemAt(event->pos());
        auto *square = dynamic_cast<Square*>(item);
        auto *photo = dynamic_cast<QGraphicsPixmapItem*>(item);

        if (square) {
            squareRoadBlock(square, !square->roadBlock);
        }
        else if (photo) {
            for (auto & stop : map->stopped) {
                if (stop.photo == photo) {
                    stop = busStopRoadBlock(stop);
                    break;
                }
            }
        }
        else {
            std::cerr << "Warning: off-road click\n";
            return;
        }
        //volání funkce kontrola autobusu
    }
}

void Scene::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton) {
        m_moving = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void Scene::mouseMoveEvent(QMouseEvent *event){
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
Scene::MoveBus()
{
    garage->MoveAllBuses(map);
}

void
Scene::StreetUpdate(float updateSlowdown, const std::string& name)
{
    map->UpdateStreet(name, updateSlowdown);
}

void
Scene::squareRoadBlock(Square* square, bool onOff)
{
    if (!square || !square->road)
        return;

    int x = square->row;
    int y = square->col;
    int start_int,end_int;
    Coordinates *start,*end;
    QMessageBox Msgbox;
    bool xy;
    Msgbox.setText("Na trase je autobus, ulice nejde uzavřít");

    if (x < 0 or x > 138)
        return;

    if (y < 0 or y > 79)
        return;

    if (x + 1 >= 0 and x + 1 < 138)
        start_int = x + 1;
    else
        start_int = x - 1;

    /* délka mezi zastávkami x osa */
    if(map->layout[start_int][y]->road) {
        start_int = x;
        end_int = x;
        xy = true;
        for(; !map->layout[end_int + 1][y]->hasStop; end_int++) {
        }
        for(; !map->layout[start_int - 1][y]->hasStop; start_int--) {
        }

        start = new Coordinates(start_int, y);
        end = new Coordinates(end_int, y);
    }
    /* y osa */
    else {
        start_int = y;
        end_int = y;
        xy = false;
        for(; !map->layout[x][end_int + 1]->hasStop; end_int++) {
        }
        for(; !map->layout[x][start_int - 1]->hasStop; start_int--) {
        }
        start = new Coordinates(x, start_int);
        end = new Coordinates(x, end_int);

    }

    //std::cerr << start_int << " " << end_int << "\n";
    /* kontrola jestli se na požadované ulici, která se chce zavřit, je autobus */
    for(auto *bus : garage->allBus) {
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

    /* přidání uzavírky */
    for(;start_int <= end_int; start_int++) {
        if (onOff) {
            if (xy) {
                map->layout[start_int][y]->roadBlock = true;
                map->layout[start_int][y]->SetColor("#FF0000");
            }
            else {
                map->layout[x][start_int]->roadBlock = true;
                map->layout[x][start_int]->SetColor("#FF0000");
            }
        }
        else {
            if (xy) {
                map->layout[start_int][y]->roadBlock = false;
                map->layout[start_int][y]->SetColor("#C0C0C0");
            }
            else {
                map->layout[x][start_int]->roadBlock = false;
                map->layout[x][start_int]->SetColor("#C0C0C0");
            }
        }
    }
}

StreetMap::stopData
Scene::busStopRoadBlock(StreetMap::stopData stop)
{
    QString path;
    QMessageBox Msgbox;
    Msgbox.setText("Na trase je autobus, zastávku nejde uzavřít");

    if(!stop.stop->roadStop) {
        for(auto *bus : garage->allBus) {
            if (bus->nextBusStop.name == stop.stop->stopName ) {
                Msgbox.exec();
                return stop;
            }
        }
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop_roadblock.jpg"));
        map->layout[stop.stop->coordinates->x][stop.stop->coordinates->y]->roadBlock = true;
    }
    else {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));
        map->layout[stop.stop->coordinates->x][stop.stop->coordinates->y]->roadBlock = false;
    }

    stop.photo = stop.stop->AddStopToScene(scene, path);

    return stop;
}

void
Scene::checkRoadBlockBus()
{
    int hourNow = Timer::GetHour();
    int minuteNow = Timer::GetMinute();
    int secNow = Timer::GetSecond();
    int i = 0;

    for( auto *bus : garage->allBus) {
        int nxt, nw, mn;
        for (; i < bus->stopInformation.size() - 2; i++) {
            nxt = bus->stopInformation[i+1].stopHour * 60 + bus->stopInformation[i+1].stopMin;
            nw  = hourNow * 60 + minuteNow;
            mn = bus->stopInformation[i].stopHour * 60 + bus->stopInformation[i].stopMin;

            if (nxt > nw and nw >= mn) {
                i++;
                break;
            }
        }

        for (; i < bus->stopInformation.size() - 2; i++) {
            if ( map->layout[bus->stopInformation[i].coordinates->x][bus->stopInformation[i].coordinates->y]->roadBlock ) {
                //uzavírka na cestě
                std::cerr << "Uzavírka zastávky " << bus->stopInformation[i].name << "\n";
                bus->roadStopOnRoad = true;
            }
            /* Ulice jde svisle */
            if (bus->stopInformation[i].coordinates->x == bus->stopInformation[i+1].coordinates->x ) {
                if ( map->layout[bus->stopInformation[i].coordinates->x][(bus->stopInformation[i].coordinates->y + bus->stopInformation[i + 1].coordinates->y) / 2 ]->roadBlock ) {
                    std::cerr << "Uzavírka mezi " << bus->stopInformation[i].name << " a " << bus->stopInformation[i+1].name << "\n";
                    //uzavírka
                    bus->roadStopOnRoad = true;
                }
            }
        }




    }
}




