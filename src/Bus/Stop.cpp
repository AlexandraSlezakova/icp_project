#include "Stop.h"

Stop::Stop(std::string name, Coordinates *coordinate)
{
    coordinates = coordinate;
    stopName = std::move(name);
    /* add stop to list */
    stopList.emplace(stopName, coordinates);
}

Stop::~Stop() =default;

Coordinates*
Stop::GetStop(const std::string& name)
{
    for (const auto& stop : stopList) {
        if (stop.first == name) {
            return stop.second;
        }
    }

    return nullptr;
}

std::map<std::string, Coordinates*> Stop::stopList{};