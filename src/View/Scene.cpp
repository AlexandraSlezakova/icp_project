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
    /* initialize map */
    map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets(Functions::GetAbsolutePath("../files/ulice.txt"));
    map->AddStops(Functions::GetAbsolutePath("../files/zastavky.txt"), scene);
    /* add it to scene */
    AddMap(map);

    /* add buses */
    auto *bus = new Bus(busId, 1);
    bus->InitBus(scene);

    BusesOnRoad.push_back(bus);
    // TODO
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

    // zde se bude získávat jen list zastávek ne vytvoření busu
    auto *bus = BusesOnRoad.back();
    bus->CreateTimetable(text, map->layout, "#FF0000");
    //bus->InitBus(scene);
    //bus->MoveBus();
}

void
Scene::MoveBus1()
{
    QTimer *timer = new QTimer(this);
    //bus->MoveBus();
    //connect(timer,SIGNAL(timeout()),))
}