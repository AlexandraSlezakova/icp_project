/**
 * @file BusRouteMap.h
 * @brief BusRouteMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUSROUTEMAP_H
#define ICP_PROJECT_BUSROUTEMAP_H

#include "../View/Street.h"
#include <QStyleOptionGraphicsItem>

class BusRouteMap : public QWidget
{
public:
    /**
     * @brief set bus route by changing color of squares
     * @param stopInformation information about bus stop
     * @param color new color of square
     */
    static void DrawLine(std::vector<Coordinates::BusStop_S> stopInformation, const QString& color);
};


#endif //ICP_PROJECT_BUSROUTEMAP_H
