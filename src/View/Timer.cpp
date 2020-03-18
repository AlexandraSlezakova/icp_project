#include "Timer.h"

int Timer::hour;
int Timer::min;
int Timer::sec;

QString
Timer::GetTime()
{
    std::stringstream ss;
    time_t now = std::time(nullptr);
    tm *ltm = localtime(&now);
    ss << ltm->tm_hour - hour << ":" << ltm->tm_min - min << ":" << ltm->tm_sec - sec;
    return QString::fromStdString(ss.str());
}

void
Timer::ChangeTime(int h, int m, int s, const QString& previousTime)
{
    std::vector<std::string> time = Functions::Split(previousTime.toStdString(), ":");
    /* find difference of old and new time */
    hour = std::stoi(time[0]) - h;
    min = std::stoi(time[1]) - m;
    sec = std::stoi(time[2]) - s;
}

void
Timer::ResetTime()
{
    hour = 0;
    min = 0;
    sec = 0;
}