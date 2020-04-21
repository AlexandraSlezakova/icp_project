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
#include "../Bus/Garage.h"

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
     * @brief update slowdown on street
     * @param updateSlowdown changed float streetSlowdown
     * @param name streetname of which slowdown should be changed
     */
    void StreetUpdate(float updateSlowdown, std::string name);

    void checkRoadBlockBus();


    QGraphicsScene *scene;
    QPlainTextEdit *text;
    QLabel *zoomText;           //!< text with information about scale scene
    int busId = 0;              //!< id of bus
    double zoom_act = 100;      //!< double expression of zoom
    Garage *garage;             //!< All busses
    StreetMap *map;             //!< map
    bool roadBlockMode = false; //!< Mode to add and sub roadblock

    int  m_originalX = 0;       //!< x mouse position to move map
    int  m_originalY = 0;       //!< y mouse position to move map
    bool m_moving = false;      //!< moving map with mouse


public slots:
    /**
     * @brief show timetable of bus number 1
     */
    void GetBus1Timetable();

    /**
    * @brief move all buses
    */
    void MoveBus();

    /**
    * @brief timer to move all buses
    */
    void MoveBuses();

    /**
    * @brief zoom in scene
    */
    void ZoomAdd();

    /**
    * @brief zooms out scene
    */
    void ZoomSub();

protected:
    /**
    * @brief zooms in and out scene by mouse
    * @param event Mouse wheel event chager
    */
    virtual void wheelEvent(QWheelEvent *event);



private:

    /**
    * @brief moving map with mouse
    * @param event mouse moving event
    */
    void mouseMoveEvent(QMouseEvent *event);

    /**
    * @brief end moving map release click on mouse
    * @param event mouse moving event
    */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
    * @brief start moving map with click on mouse and add roadblock on map
    * @param event mouse moving event
    */
    void mousePressEvent(QMouseEvent *event);

    /**
    * @brief add and delete roadblock between two stops
    * @param square information on clicked square
    * @param onOff mood on add or delete roadblock
    */
    void squareRoadBlock(Square *square, bool onOff);

    /**
    * @brief add and delete roadblock on stop
    * @param stop roadblock stop
    * @return stopData to update data
    */
    StreetMap::stopData busStopRoadBlock(StreetMap::stopData stop);

};

#endif // SCENE_H
