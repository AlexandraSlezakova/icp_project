/**
 * @file Square.h
 * @brief Square header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QBrush>

#define SQUARE_SIZE     20
#define X    138
#define Y    79

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

    int row = 0;                       //!< row
    int col = 0;                       //!< column
    bool hasStop = false;              //!< information about bus stop on square
    bool roadBlock = false;            //!< information about roadblock on square
    bool road = false;                 //!< information about type square ( road / grass )
    static Square *layout[X][Y];       //!< layout with squares

private:
    QColor color_;              //!< color of square
    QBrush brush;
};

#endif // SQUARE_H
