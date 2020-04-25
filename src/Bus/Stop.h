/**
 * @file Stop.h
 * @brief Stop header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef STOP_H
#define STOP_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QtGui/QPainter>
#include <QtWidgets>
#include "../View/Coordinates.h"

#define SQUARE_SIZE     20

class Stop
{
public:
    /**
     * @brief initial setup of bus stop
     * @param stopName bus stop name
     * @param coordinates coordinates of bus stop
     */
     Stop(std::string name, Coordinates::Coordinates_S coordinate);

    ~Stop();

    /**
     * @brief adds image of bus stop on square
     * where bus stops of each street are
     * @param scene graphics scene
     * @param pathToFile path to image
     * @return to save stop photo
     */
    QGraphicsPixmapItem* AddStopToScene(QGraphicsScene *scene, const QString& pathToFile);

    Coordinates::Coordinates_S coordinates;  //!< coordinates of bus stop
    std::string stopName;                    //!< bus stop name
    bool roadStop;                           //!< save information about road stop on Stops
};

#endif // STOP_H
