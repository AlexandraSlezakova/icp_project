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

class Stop
{
public:
    /**
     * @brief initial setup of bus stop
     * @param stopName bus stop name
     * @param coordinates coordinates of bus stop
     */
     Stop(std::string name, Coordinates *coordinate);

    ~Stop();

    /**
     * @brief get coordinates of stop by name
     * @param name name of stop
     * @return coordinates of stop or nullptr
     */
    static Coordinates *GetStop(const std::string& name);

    Coordinates *coordinates;                               //!< coordinates of bus stop
    std::string stopName;                                   //!< bus stop name
    static std::map<std::string, Coordinates*> stopList;    //!< list of stops

private:
    QBrush brush;
};

#endif // STOP_H
