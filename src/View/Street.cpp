#include "Street.h"

Street::Street(std::string s, Coordinates *start, Coordinates *end)
{
    this->name = std::move(s);
    this->start = start;
    this->end = end;
}

Street::~Street() =default;

void
Street::SetStop(Stop *stop)
{
    busStopsList.push_back(stop);
}