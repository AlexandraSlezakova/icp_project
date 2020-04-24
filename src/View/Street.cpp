#include "Street.h"

Street::Street(std::string s, Coordinates::Coordinates_S start, Coordinates::Coordinates_S end, float slowdown)
{
    this->name = std::move(s);
    this->start = start;
    this->end = end;
    this->slowdown = previousSlowdown = slowdown;
}

Street::~Street() =default;