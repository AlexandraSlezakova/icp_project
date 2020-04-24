#include "Stop.h"

Stop::Stop(std::string name, Coordinates::Coordinates_S coordinate)
{
    coordinates = coordinate;
    stopName = std::move(name);
    roadStop = false;
}

Stop::~Stop() =default;

QGraphicsPixmapItem*
Stop::AddStopToScene(QGraphicsScene *scene, const QString& pathToFile)
{
    auto *item = new QGraphicsPixmapItem(QPixmap(pathToFile));
    item->setToolTip(QString::fromStdString(stopName));
    scene->addItem(item);
    item->setScale(0.05);
    /* put image in the center of square */
    item->setPos(coordinates.x * SQUARE_SIZE + 2, coordinates.y * SQUARE_SIZE + 1);
    return item;
}