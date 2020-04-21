/**
 * @file Bus.h
 * @brief Bus header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUS_H
#define ICP_PROJECT_BUS_H

#include "BusRouteMap.h"
#include <sstream>
#include <QPlainTextEdit>

class Bus
{
public:
    int id_;                                                //!< id of bus
    int busNumber_;                                         //!< number of bus
    int iteration;
    std::vector<Coordinates::BusStop_S> stopInformation;    //!< bus stop coordinates and time
    Coordinates *busPosition;                               //!< position of bus
    QGraphicsPixmapItem *busPhoto{};                             //!< picture of bus
    Coordinates::BusStop_S nextBusStop, currentBusStop;     //!< current and next bus stop


    /**
     * @brief bus constructor
     * @param id id of bus
     * @param busNumber number of bus
     * @param position position
     */
    Bus(int id, int busNumber, Coordinates *position);

    ~Bus() = default;

    /**
     * @brief initialize bus
     * put image of little bus on scene, set scale and initial position
     * @param scene
     */
    void InitBus(QGraphicsScene *scene);

    /**
     * @brief get coordinates of bus
     * if secNow is greater than 30 and we are moving along the x axis, set x coordinate near to next bus stop
     * otherwise set it near to current bus stop, same with the y axis
     * @param secNow seconds right now
     * @param isC flag if we are moving along the x or y axis
     * @param halfWay middle of two bus stops
     * @param current current bus stop coordinate
     * @param next next bus stop coordinate
     * @return x or y position of bus
     */
     int GetCoordinate(int hourNow, int minNow, int secNow, int isC, const Coordinates::BusStop_S& current,
                       const Coordinates::BusStop_S& next);

    /**
     * @brief set position of bus according its timetable
     */
    void MoveBus();

    /**
     * @brief insert coordinates of bus stops to list and append bus timetable to textarea
     * @param color color of bus route
     */
    void CreateTimetable(QString& color);

    /**
     * @brief initialize text area for timetable
     * @param parent parent widget
     * @param width width of textarea
     * @param height height of textarea
     */
    static void InitTimetableArea(QWidget *parent, int width, int height);

    /**
     * @brief clear text area with timetable of bus
     */
    void ClearTextArea();

private:
    static QPlainTextEdit *textArea;   //!< text area for bus timetable

    /**
     * @brief load timetable of bus from file and save all information
     */
    void LoadTimetable();

};


#endif //ICP_PROJECT_BUS_H
