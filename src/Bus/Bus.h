/**
 * @file Bus.h
 * @brief Bus header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUS_H
#define ICP_PROJECT_BUS_H

#include "../View/Square.h"
#include "BusRouteMap.h"
#include <sstream>
#include <QPlainTextEdit>

class Bus
{
public:
    int id_;                    //!< id of bus
    std::vector<Coordinates::BusStop_S> stopInformation;    //!< bus stop coordinates and time
    Coordinates *busPosition;
    int rotation;
    int avgspeed;
    Coordinates::BusStop_S nextBusStop, currentBusStop;

    /**
     * @brief bus constructor
     * @param id id of bus
     * @param busNumber number of bus
     */
    explicit Bus(int id, int busNumber, Coordinates *position);

    ~Bus() = default;

    /**
     * @brief initialize bus
     * put image of little bus on scene, set scale and initial position
     * @param scene
     */
    void InitBus(QGraphicsScene *scene);

    /**
     * @brief set position of bus according its timetable
     */
    void MoveBus();

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
     int GetCoordinate(int hourNow, int minNow, int secNow, int isC, Coordinates::BusStop_S current,Coordinates::BusStop_S next);

    /**
     * @brief insert coordinates of bus stops to list and append bus timetable to textarea
     * @param text textarea
     * @param layout layout with squares
     * @param color color of bus route
     */
    void CreateTimetable(QPlainTextEdit *text, Square *layout[X][Y], const QString& color);

    Square *currentSquare;                           //!< square where bus is

private:

    int busNumber_;             //!< number of bus

    QGraphicsPixmapItem *bus;   //!< picture of bus
};


#endif //ICP_PROJECT_BUS_H
