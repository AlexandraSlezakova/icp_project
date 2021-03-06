/**
 * @file Bus.h
 * @brief Bus header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUS_H
#define ICP_PROJECT_BUS_H

#include <sstream>
#include <QPlainTextEdit>
#include "../View/StreetMap.h"
#include "BusRouteMap.h"
#include "../View/Timer.h"
#include <iomanip>
#include <cmath>

class Bus
{
public:
    int id_;                                                //!< id of bus
    int busNumber_;                                         //!< number of bus
    int deleteBus = 0;                                      //!< flag if bus should be deleted
    std::vector<Coordinates::BusStop_S> stopInformation;    //!< bus stop coordinates and time
    Coordinates::Coordinates_S busPosition;                 //!< position of bus
    QGraphicsPixmapItem *busPhoto = nullptr;                //!< picture of bus
    Coordinates::BusStop_S nextBusStop, currentBusStop;     //!< current and next bus stop
    QPlainTextEdit *textArea = nullptr;

    std::vector<Coordinates::BusStop_S> newStopInformation; //!< new bus stop coordinates and time
    bool roadStopOnRoad;                                    //!< value if its roadblock on busway
    bool stopMoving = false;                                //!< for stop moving with bus, because roadblock

    int stopHour{};                                           //!< hour when the bus is at a stop
    int stopMin{};                                            //!< minute when the bus is at a stop
    int pastStops;                                          //!< counter on stops


    /**
     * @brief bus constructor
     * @param id id of bus
     * @param busNumber number of bus
     * @param busIteration counting bus same busline
     */
    Bus(int id, int busNumber, int busCount = 0);

    ~Bus();

    /**
     * @brief initialize bus photo
     * put image of little bus on scene, set scale and initial position
     * @param scene graphics scene
     */
    void InitBusPhoto(QGraphicsScene *scene, const char *imagePath, int x, int y);

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
     * @param textArea text area for timetable
     */
    void CreateTimetable(QString& color, QPlainTextEdit *textArea);

    /**
     * @brief change rotation of bus image
     * @param x x coordinate
     * @param y y coordinate
     * @param next next bus stop
     */
    void BusRotation(int x, int y, const Coordinates::BusStop_S &next) const;

    /**
     * @brief change current and next busstop
     */
    void GetBusStops();

private:
    /**
     * @brief load timetable of bus from file and save all information
     */
    void LoadTimetable(int busCount);
};


#endif //ICP_PROJECT_BUS_H
