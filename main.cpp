#include "main.h"



int main() {
    std::string streetName;

    auto map = new StreetMap();
    map->AddStreets("/home/alexandra/Projects/icp_project/ulice.txt");
    map->AddStops("/home/alexandra/Projects/icp_project/zastavky.txt");

    /* add bus stops */


    return 0;
}
