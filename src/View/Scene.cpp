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
    setFixedSize(X * SQUARE_SIZE, Y * SQUARE_SIZE);
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
    /*if (event->button() == Qt::RightButton)
    {
        m_moving = true;
        m_originalX = event->x();
        m_originalY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    else*/
    if (event->button() == Qt::LeftButton && roadBlockMode) {
        QGraphicsItem *item = itemAt(event->pos());
        auto *square = dynamic_cast<Square*>(item);
        auto *photo = dynamic_cast<QGraphicsPixmapItem*>(item);

        if (square) {
            squareRoadBlock(square, !square->roadBlock);
            std::cerr << "square \n";
        }
        else if (photo) {
            for (auto & i : map->stopped) {
                if (i.photo == photo) {
                    i = busStopRoadBlock(i);
                }
            }
        }
        else {
            std::cerr << "Warning: off-road click\n";
        }
    }
}

/*void Scene::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton)
    {
        m_moving = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void Scene::mouseMoveEvent(QMouseEvent *event){
    if (m_moving)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_originalX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_originalY));
        m_originalX = event->x();
        m_originalY = event->y();
        event->accept();
        return;
    }
    event->ignore();

}*/

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
Scene::StreetUpdate(float updateSlowdown, std::string name)
{
    map->UpdateStreet(name, updateSlowdown);
}

void
Scene::RoadBlock(int x, int y)
{
}

void
Scene::squareRoadBlock(Square* square, bool onOff)
{
    if (!square)
        return;

    int x = square->row;
    int y = square->col;

    if (x < 0 or x >= 138)
        return;

    if (y < 0 or y >= 79)
        return;

    if (square->road) {

         if (!square->hasStop and square->roadBlock != onOff) {

            if (onOff) {
                square->roadBlock = true;
                map->layout[x][y]->SetColor("#FF0000");
            }
            else {
                square->roadBlock = false;
                map->layout[x][y]->SetColor("#C0C0C0");
            }

            squareRoadBlock(map->layout[x + 1][y], onOff);
            squareRoadBlock(map->layout[x - 1][y], onOff);
            squareRoadBlock(map->layout[x][y + 1], onOff);
            squareRoadBlock(map->layout[x][y - 1], onOff);
        }
    }
}

StreetMap::stopData
Scene::busStopRoadBlock(StreetMap::stopData stop)
{
    QString path;

    if(!stop.stop->roadStop) {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop_roadblock.jpg"));
        stop.stop->roadStop = true;
    }
    else {
        path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));
        stop.stop->roadStop = false;
    }

    stop.photo = stop.stop->AddStopToScene(scene, path);

    return stop;
}




