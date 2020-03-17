#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "../Stop.h"
#define SQUARE_SIZE     20
class Square : public QGraphicsRectItem
{
public:
    explicit Square(QGraphicsRectItem *parent = nullptr);

    void SetColor(const QString &name);

    int row = 0;                //!< row
    int col = 0;                //!< column
    bool hasStop = false;       //!< information about bus stop on square

private:
    QColor color_;
    QBrush brush;
};

#endif // SQUARE_H
