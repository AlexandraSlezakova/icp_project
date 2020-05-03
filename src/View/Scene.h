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

    ~Scene();

    /**
     * @brief create map with streets, bus stops and buses
     */
    void CreateMap();

    /**
     * @brief add whole map to scene
     */
    void AddSquares();

    /**
     * @brief set up view and add scene
     */
    void SetUpView();

    /**
     * @brief move all buses
     */
    void MoveBuses();

    /**
     * @brief update slowdown on street
     * @param updateSlowdown changed float streetSlowdown
     * @param name street name of which slowdown should be changed
     */
    void StreetUpdate(float updateSlowdown, const std::string& name);

    /**
     * @brief add buses to scene
     */
    void AddBuses();

    void CheckRoadBlockBus();

    void MapClean();

    /**
     * @brief initialize text area for timetable
     * @param parent parent widget
     * @param width width of textarea
     * @param height height of textarea
     */
    void InitTimetableArea(QWidget *parent, int width, int height);

    QGraphicsScene *graphicsScene;  //!< graphics scene
    QPlainTextEdit *textArea;       //!< text area for bus timetable
    QLabel *zoomText;               //!< text with information about scale scene
    double zoom_act = 100;          //!< double expression of zoom
    Garage garage;                  //!< all buses
    StreetMap *map;                 //!< map
    bool roadBlockMode = false;     //!< Mode to add and sub roadblock

    int  m_originalX = 0;           //!< x mouse position to move map
    int  m_originalY = 0;           //!< y mouse position to move map
    bool m_moving = false;          //!< moving map with mouse
    Bus *markedBus = nullptr;
    int busId = 0;                  //!< bus id


protected:
    /**
    * @brief zooms in and out scene by mouse
    * @param event mouse wheel event changer
    */
    void wheelEvent(QWheelEvent *event) override;

public slots:
    /**
    * @brief zoom in scene
    */
    void ZoomAdd();

    /**
    * @brief zooms out scene
    */
    void ZoomSub();

private:
    /**
    * @brief moving map with mouse
    * @param event mouse moving event
    */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
    * @brief end moving map release click on mouse
    * @param event mouse moving event
    */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
    * @brief start moving map with click on mouse and add roadblock on map
    * @param event mouse moving event
    */
    void mousePressEvent(QMouseEvent *event) override;

    /**
    * @brief add and delete roadblock between two stops
    * @param square information on clicked square
    * @param onOff mood on add or delete roadblock
    */
    void SquareRoadBlock(Square *square, bool onOff); 

    /**
     * @brief show route of clicked bus
     * @param photo bus photo
     */
    void ShowRoute(QGraphicsItem *photo);
    
    /**
    * @brief add and delete roadblock on stop
    * @param stop roadblock stop
    * @return stopData to update data
    */
    StreetMap::stopData BusStopRoadBlock(StreetMap::stopData stop);

    void GetStreetPositionBetweenStops(Square *square, Coordinates::Coordinates_S *start, Coordinates::Coordinates_S *end,
                                       int* startInt, int* endInt, bool* xy);

    /**
     * @brief add the bus one by one to scene at the beginning of the program execution
     * according to current time
     */
    void AddBusOneByOne();

    void PlottingRouteBetweenStops(const Coordinates::BusStop_S &first, const Coordinates::BusStop_S &second,QString colour) const;

    void BusPick(Bus *b);

    void RoadStopBusService(Square *square, QGraphicsPixmapItem *photo);

    void ContinueBusRoute(Coordinates::BusStop_S &information);
};

#endif // SCENE_H
