#include "Square.h"

Square::Square(QGraphicsRectItem *parent) : QGraphicsRectItem(parent)
{
    /* square size 75 */
    setRect(0, 0, 75, 75);
    brush().setStyle(Qt::SolidPattern);
}

void
Square::setColor(QColor color)
{
    this->color = color;
}
