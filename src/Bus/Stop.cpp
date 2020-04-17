#include "Stop.h"

Stop::Stop(std::string name, Coordinates *coordinate)
{
    coordinates = coordinate;
    stopName = std::move(name);
    /* add stop to list */
    stopList.emplace(stopName, coordinates);
    roadStop = false;
}

Stop::~Stop() =default;

Coordinates*
Stop::GetStop(const std::string& name)
{
    for (const auto& stop : stopList) {
        if (stop.first == name) {
            return stop.second;
        }
    }

    return nullptr;
}

std::map<std::string, Coordinates*> Stop::stopList{};


QGraphicsPixmapItem*
Stop::AddStopToScene(QGraphicsScene *scene, const QString& pathToFile)
{
    auto *item = new QGraphicsPixmapItem(QPixmap(pathToFile));
    item->setToolTip(QString::fromStdString(stopName));
    scene->addItem(item);
    item->setScale(0.05);
    /* put image in the center of square */
    item->setPos(coordinates->x * SQUARE_SIZE + 2, coordinates->y * SQUARE_SIZE + 1);
    return item;
}