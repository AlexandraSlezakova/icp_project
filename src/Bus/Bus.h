/**
 * @file Bus.h
 * @brief Bus header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_BUS_H
#define ICP_PROJECT_BUS_H

#include "../View/Square.h"
#include <sstream>
#include <QPlainTextEdit>

class Bus
{
public:
    explicit Bus(int id);

    ~Bus() = default;

    void SetCurrentSquare(Square *square);

    void CreateTimetable(QPlainTextEdit *text);

    Square *currentSquare;                          //!< square where bus is
    std::vector<Coordinates*> stopCoordinates;       //!< coordinates of all stops of bus

private:
    int id_;             //!< id of bus
};


#endif //ICP_PROJECT_BUS_H
