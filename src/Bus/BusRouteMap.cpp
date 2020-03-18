#include "BusRouteMap.h"

BusRouteMap::BusRouteMap(QWidget *parent) : QWidget(parent)
{
//    timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, QOverload<>::of(&BusRouteMap::update));
//    timer->start(1000);
//
//    setWindowTitle(tr("Analog Clock"));
//    resize(200, 200);
}

void
BusRouteMap::paintEvent(QPaintEvent *event)
{
    //QTime time = QTime::currentTime();
}

void
BusRouteMap::DrawLine(Square *layout[X][Y])
{
    //auto *bus = new Bus(1);
    //bus->CreateTimetable();

}

