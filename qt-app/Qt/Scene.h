#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "../StreetMap.h"

class Scene : public QGraphicsView
{
public:
    Scene(QWidget *parent = nullptr);

    void CreateMap();

    void AddMap(StreetMap *map);

    void AddBusStops(StreetMap *map);

    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // SCENE_H
