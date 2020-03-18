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
    static void DrawLine(Square *layout[X][Y]);

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif //ICP_PROJECT_BUSROUTEMAP_H
