/**
 * @file Timer.h
 * @brief Timer header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef ICP_PROJECT_TIMER_H
#define ICP_PROJECT_TIMER_H

#include <QMainWindow>
#include <QApplication>
#include <ctime>
#include <iostream>
#include <sstream>
#include "../Functions.h"

class Timer : public QWidget
{
public:
    /**
     * @brief get current time
     * @return current time
     */
    static QString GetTime();

    /**
     * @brief change time
     * @param h new hour
     * @param m new minute
     * @param s new second
     * @param previousTime previous time
     */
    static void ChangeTime(int h, int m, int s, const QString& previousTime);

    /**
     * @brief get seconds of current time
     * @return seconds
     */
    static int GetSecond();

    /**
     * @brief get minutes of current time
     * @return minutes
     */
    static int GetMinute();

    /**
     * @brief get hour of current time
     * @return minutes
     */
    static int GetHour();

    /**
     * @brief reset timer
     */
    static void ResetTime();

private:
    static int hour;        //!< hour
    static int min;         //!< minute
    static int sec;         //!< second
    static tm *ltm;

};


#endif //ICP_PROJECT_TIMER_H
