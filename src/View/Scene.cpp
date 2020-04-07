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
    Bus* bus = garage->GetBus(busId, 1);
    bus->CreateTimetable(text, map->layout, "#FF0000");
    bus->InitBus(scene);
    bus->MoveBus();
}

void
Scene::MoveBuses()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(MoveBus()));
    timer->start(1000); //time specified in ms
}

void Scene::wheelEvent(QWheelEvent *event) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.15;

    if(event->delta() > 0)
    {
        scale(scaleFactor,scaleFactor);
        zoom_act *= scaleFactor;
    }

    else
    {
        scale(1/scaleFactor,1/scaleFactor);
        zoom_act = zoom_act*(1/scaleFactor);
    }
    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void Scene::ZoomAdd() {
    double scaleFactor = 1.15;
    scale(scaleFactor,scaleFactor);
    zoom_act *= scaleFactor;
    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void Scene::ZoomSub() {
    double scaleFactor = 1.15;
    scale(1/scaleFactor,1/scaleFactor);
    zoom_act = zoom_act*(1/scaleFactor);
    zoomText->setText("Actual zoom = " + QString::number(zoom_act,'f',2));
}

void Scene::MoveBus() {
    Bus* bus = garage->GetBus(0, 1);
    bus->MoveBus();
}
