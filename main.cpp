#include "main.h"

int main() {
    std::string streetName;

    auto map = new StreetMap();
    map->AddStreets("/home/alexandra/Projects/icp_project/files/ulice.txt");
    map->AddStops("/home/alexandra/Projects/icp_project/files/zastavky.txt");

    return 0;
}
