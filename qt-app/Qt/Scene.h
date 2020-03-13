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

    QGraphicsScene *scene;
};

#endif // SCENE_H
