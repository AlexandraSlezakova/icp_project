/**
 * @file Scene.h
 * @brief Scene header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets>
#include <QWidget>
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
    explicit Scene(QWidget *parent = nullptr);

    /**
     * @brief create map with streets, bus stops and buses
     */
    void CreateMap();

    /**
     * @brief add whole map to scene
     * @param streetMap map of streets
     */
    void AddMap(StreetMap *streetMap);

    /**
     * @brief set up view and add scene
     */
    void SetUpView();

    /**
    * @brief zoom in scene
     */
    void zoomAdd();

    /**
    * @brief zooms out scene
    */
    void zoomSub();

    QGraphicsScene *scene;
    QPlainTextEdit *text;
    QLabel *zoomText;
    int busId = 0;          //!< id of bus
    double zoom_act = 100;  //!< double expression of zoom



protected:
    /**
    * @brief zooms in and out scene by mouse
    * @param event Mouse wheel event chager
    */
    virtual void wheelEvent(QWheelEvent *event);


public slots:
    /**
     * @brief show timetable of bus number 1
     */
    void GetBus1Timetable();

    void MoveBus1();
private:
    StreetMap *map;
};

#endif // SCENE_H
