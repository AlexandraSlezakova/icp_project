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
    auto *map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets(Functions::GetAbsolutePath("../files/ulice.txt"));
    map->AddStops(Functions::GetAbsolutePath("../files/zastavky.txt"));

    AddMap(map);
    AddBusStops(map);

    /* add buses */
    // TODO
    auto *busRouteMap = new BusRouteMap();
}

void
Scene::AddMap(StreetMap *map)
{
    for (auto & x : map->layout) {
        for (auto & y : x) {
            scene->addItem(y);
        }
    }
}

void
Scene::AddBusStops(StreetMap *map)
{
    for (auto& x : map->Map) {
        for (auto &y : x) {
            for (auto &street : y) {
                IF(street, street->AddStopsToScene(scene))
            }
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
    auto *bus = new Bus(1);
    bus->CreateTimetable(text);
}