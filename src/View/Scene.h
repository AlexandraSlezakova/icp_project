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
     * @param currentMinute current minute
     */
    void AddBuses();

    /**
     * @brief add the bus one by one to scene at the beginning of the program execution
     * according to current time
     */
    void AddBusOneByOne();

    /**
     * @brief show map only with road, roadblock and slowdown
     */
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
    Bus *seenBus = nullptr;         //!< already seen bus in ShowRoute method
    bool roadBlockMode = false;     //!< Mode to add and sub roadblock

    int  m_originalX = 0;           //!< x mouse position to move map
    int  m_originalY = 0;           //!< y mouse position to move map
    bool m_moving = false;          //!< moving map with mouse
    Bus *markedBus = nullptr;       //!< a bus that changes route
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

    /**
    * @brief Get coordinate between 2 stops
    * @param square clicked square
    * @param start starting coordinate (return)
    * @param end ending coordinate (return)
    * @param startInt x or y starting coordinate
    * @param endInt x or y ending coordinate
    * @param xy x or y axis
    */
    void GetStreetPositionBetweenStops(Square *square, Coordinates::Coordinates_S *start, Coordinates::Coordinates_S *end,
                                       int* startInt, int* endInt, bool* xy);

    /**
    * @brief Change color on map between two stops
    * @param first first stop
    * @param second second stop
    * @param colour the color to which the path changes
    */
    void PlottingRouteBetweenStops(const Coordinates::BusStop_S &first, const Coordinates::BusStop_S &second,QString colour) const;

    /**
    * @brief Pick bus in RoadBlockMode, assign to markedBus
    * @param bus picked bus
    */
    void BusPick(Bus *b);

    /**
    * @brief Service marked bus, clicking bus road
    * @param square clicked square
    * @param photo clicked busstop or new bus to pick new markedBus
    */
    void RoadStopBusService(Square *square, QGraphicsPixmapItem *photo);

    /**
    * @brief connection to the previous bus journey
    * @param information stop from which to continue
    */
    void ContinueBusRoute(Coordinates::BusStop_S &information);
};

#endif // SCENE_H
