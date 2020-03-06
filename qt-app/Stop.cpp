#include "Stop.h"

Stop::Stop(std::string stopName, Coordinates *coordinates)
{
    this->coordinates = coordinates;
    this->stopName = std::move(stopName);
}

Stop::~Stop() =default;
