#include "Scene.h"

Scene::Scene(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene;
    /* size of board */
    scene->setSceneRect(0,0,2000,1000);
}

void
Scene::CreateMap()
{
    /* initialize map */
    auto map = new StreetMap();
    /* initialize streets and bus stops */
    map->AddStreets("/home/alexandra/Projects/icp_project/files/ulice.txt");
    map->AddStops("/home/alexandra/Projects/icp_project/files/zastavky.txt");

    /* add whole map to scene */
    for (auto & x : map->layout) {
        for (auto & y : x) {
            scene->addItem(y);
        }
    }

    /* create view and add scene */
    view = new QGraphicsView();
    view->setScene(scene);

    /* add buses and bus stops */
    // TODO
}
