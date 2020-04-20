#include "Street.h"

Street::Street(std::string s, Coordinates *start, Coordinates *end, float slowdown)
{
    this->name = std::move(s);
    this->start = start;
    this->end = end;
    this->slowdown = previousSlowdown = slowdown;
}

Street::~Street() =default;

void
Street::SetStop(Stop *stop)
{
    busStopsList.push_back(stop);
}