#include "Scene.h"

Scene::Scene(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    /* size of board */
    scene->setSceneRect(0,0,X * SQUARE_SIZE + 50, Y * SQUARE_SIZE + 50);
}

void
Scene::CreateMap()
{
    /* initialize map */
    auto *map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets("/home/alexandra/Projects/icp_project/files/ulice.txt");
    map->AddStops("/home/alexandra/Projects/icp_project/files/zastavky.txt");
    /* add whole map to scene */
    AddMap(map);
    /* add image of bus stop on map */
    AddBusStops(map);

    /* add buses */
    // TODO

    /* create view and add scene */
    view = new QGraphicsView();
    view->setScene(scene);
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