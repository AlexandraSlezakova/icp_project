/**
 * @file Scene.h
 * @brief Scene header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "StreetMap.h"

class Scene : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief scene constructor
     * @param parent parent widget
     */
    Scene(QWidget *parent = nullptr);

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

    /**
     * @brief set up view and add scene
     */
    void SetUpView();

    QGraphicsScene *scene;
    QPlainTextEdit *text;

public slots:
    void GetBus1Timetable();
};

#endif // SCENE_H
