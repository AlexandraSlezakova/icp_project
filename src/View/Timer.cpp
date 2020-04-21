#include "Timer.h"

tm *Timer::ltm;

int
Timer::GetHour()
{
    return ltm->tm_hour;
}

int
Timer::GetMinute()
{
    return ltm->tm_min;
}

int
Timer::GetSecond()
{
    return ltm->tm_sec;
}

QString
Timer::GetTime()
{
    std::stringstream ss;
    time_t now = std::time(nullptr);
    std::string h, m, s;

    /* initialize structure with system time */
    if (!ltm) {
        ltm = localtime(&now);
    }
    else {
        ltm->tm_sec++;

        if (ltm->tm_sec == 60) {
            ltm->tm_min++;

            if (ltm->tm_min == 60) {
                ltm->tm_hour++;

                if (ltm->tm_hour == 24) {
                    ltm->tm_hour = 0;
                }

                ltm->tm_min = 0;
            }

            ltm->tm_sec = 0;
        }
    }

    /* append 0 at the begging of time */
    IF(ltm->tm_hour < 10, h = "0")
    IF(ltm->tm_min < 10, m = "0")
    IF(ltm->tm_sec < 10, s = "0")

    ss << h << ltm->tm_hour << ":" << m << ltm->tm_min << ":" << s << ltm->tm_sec;
    return QString::fromStdString(ss.str());
}

void
Timer::ChangeTime(int h, int m, int s)
{
    if (m >= 60) {
        h++;
        m -= 60;
    }

    if (h >= 24) {
        m = s= h = 0;
    }

    if (s >= 60) {
        m++;
        s -= 60;
    }

    ltm->tm_hour = h;
    ltm->tm_min = m;
    ltm->tm_sec = s;
}

void
Timer::ResetTime()
{
    time_t now = std::time(nullptr);
    ltm = localtime(&now);
}