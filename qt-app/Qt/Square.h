#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QBrush>
#define SQUARE_SIZE     12
class Square : public QGraphicsRectItem
{
public:
    Square(QGraphicsRectItem *parent = nullptr);

    void setColor(QColor color);

    int row, col;   //!< row and column

private:
    QColor color;
};

#endif // SQUARE_H
