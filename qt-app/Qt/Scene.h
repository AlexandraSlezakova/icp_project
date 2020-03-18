/**
 * @file Scene.h
 * @brief Scene header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "../StreetMap.h"

class Scene : public QGraphicsView
{
public:
    /**
     * @brief scene constructor
     * @param parent parent widget
     */
    Scene(QGraphicsView *parent = nullptr);

    /**
     * @brief create map with streets, bus stops and buses
     */
    void CreateMap();

    /**
     * @brief add whole map to scene
     * @param map map of streets
     */
    void AddMap(StreetMap *map);

    /**
     * @brief add image of bus stop to scene
     * @param map map of streets
     */
    void AddBusStops(StreetMap *map);

    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif // SCENE_H
