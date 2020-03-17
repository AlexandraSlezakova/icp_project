#include "Square.h"

#include <utility>

Square::Square(QGraphicsRectItem *parent) : QGraphicsRectItem(parent)
{
    setRect(0, 0, SQUARE_SIZE, SQUARE_SIZE);
    brush().setStyle(Qt::SolidPattern);
    //setZValue(-1);
}

void
Square::setColor(QColor color)
{
    this->color = std::move(color);
}
