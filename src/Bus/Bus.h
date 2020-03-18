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

    /**
     * @brief bus constructor
     * @param id id of bus
     */
    explicit Bus(int id);

    ~Bus() = default;

    void SetCurrentSquare(Square *square);

    /**
     * @brief insert coordinates of bus stops to list and append bus timetable to textarea
     * @param text textarea
     */
    void CreateTimetable(QPlainTextEdit *text);

    Square *currentSquare;                           //!< square where bus is
    std::vector<Coordinates*> stopCoordinates;       //!< coordinates of all stops of bus

private:
    int id_;             //!< id of bus
};


#endif //ICP_PROJECT_BUS_H
