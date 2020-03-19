#include "Square.h"

Square::Square(QGraphicsRectItem *parent) : QGraphicsRectItem(parent)
{
    /* set square size */
    setRect(0, 0, SQUARE_SIZE, SQUARE_SIZE);
    brush.setStyle(Qt::SolidPattern);
    setZValue(-2);
}

void
Square::SetColor(const QString &name)
{
    color_.setNamedColor(name);
    brush.setColor(color_);
    setBrush(brush);
}