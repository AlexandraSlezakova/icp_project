/**
 * @file BusRouteMap.h
 * @brief BusRouteMap header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUSROUTEMAP_H
#define ICP_PROJECT_BUSROUTEMAP_H

#include "../View/Street.h"
#include <QTimer>

class BusRouteMap : public QWidget
{
    Q_OBJECT;
public:
    explicit BusRouteMap(QWidget *parent = nullptr);

    ~BusRouteMap() = default;

    void DrawLine(Square *layout[X][Y]);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *timer;
};


#endif //ICP_PROJECT_BUSROUTEMAP_H
