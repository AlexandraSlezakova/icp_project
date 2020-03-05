#include "StreetMap.h"

StreetMap::StreetMap()
{
    for (auto & x : Map) {
        for (auto & y : x) {
            y.push_back(nullptr);
        }
    }
}


StreetMap::~StreetMap()
{
    for (auto & x : Map) {
        for (auto & y : x) {
            IF(y.size() > 1, delete y.front())
            delete y.back();
        }
    }
}

bool
StreetMap::AddStreet(Street *s)
{
    int counter = 0;

    for (int x = s->start->x; x <= s->end->x; x++) {
        for (int y = s->start->y; y <= s->end->y; y++) {
            /* only two streets can cross at a point */
            std::vector<Street *> &positionOnMap = Map[x][y];
            IF(positionOnMap.front() != nullptr && counter >= 0, counter++)

            /* restart for loop */
            if (counter > 1) {
                x = s->start->y == s->end->y ? s->start->x - 1 : s->start->x;
                y = s->start->x == s->end->x ? s->start->y - 1 : s->start->y;
                counter = -1;
            } /* erase street from map */
            else if (counter == -1) {
                IF(positionOnMap.back()->name == s->name, positionOnMap.pop_back())
                IF(positionOnMap.empty(), positionOnMap.push_back(nullptr))
            } /* add street on map */
            else {
                /* erase nullptr */
                IF(positionOnMap.front() == nullptr, positionOnMap.erase(positionOnMap.begin()))
                positionOnMap.push_back(s);
            }
        }
    }

    return counter != -1;
}

Street*
StreetMap::GetStreet(const std::string& name)
{
    for (auto & x : Map) {
        for (auto & y : x) {
            if (!y.empty()) {
                IF_RETURN(y.front() && y.front()->name == name, y.front())
                /* two crossed streets */
                if (y.size() > 1) {
                    IF_RETURN(y.back()->name == name, y.back())
                }
            }
        }
    }

    return nullptr;
}