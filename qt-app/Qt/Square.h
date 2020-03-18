/**
 * @file Square.h
 * @brief Square header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QBrush>
#include "../Stop.h"

#define SQUARE_SIZE     20

class Square : public QGraphicsRectItem
{
public:
    /**
     * @brief square constructor
     * @param parent parent widget
     */
    explicit Square(QGraphicsRectItem *parent = nullptr);

    /**
     * @brief set color of square
     * @param name color in hex format
     */
    void SetColor(const QString &name);

    int row = 0;                //!< row
    int col = 0;                //!< column
    bool hasStop = false;       //!< information about bus stop on square

private:
    QColor color_;              //!< color of square
    QBrush brush;
};

#endif // SQUARE_H
