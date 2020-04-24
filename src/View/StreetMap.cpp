#include "StreetMap.h"

StreetMap::StreetMap(QGraphicsRectItem *parent) : QGraphicsRectItem(parent)
{
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            Map[x][y].push_back(nullptr);
            auto *square = new Square();
            square->row = x;
            square->col = y;
            square->setPos(x * SQUARE_SIZE, y * SQUARE_SIZE);
            square->SetColor("#CCFFCC");
            Square::layout[x][y] = square;
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
    static std::string crossedStreet;

    for (int x = s->start.x; x <= s->end.x; x++) {
        for (int y = s->start.y; y <= s->end.y; y++) {
            /* only two streets can cross at a point */
            std::vector<Street *> &positionOnMap = Map[x][y];
            IF(positionOnMap.front() != nullptr && counter >= 0, counter++)

            IF(counter == 1 && crossedStreet.empty(), crossedStreet = positionOnMap.front()->name)

            /* restart for loop */
            if (counter > 1) {
                if (crossedStreet == positionOnMap.front()->name) {
                    x = s->start.y == s->end.y ? s->start.x - 1 : s->start.x;
                    y = s->start.x == s->end.x ? s->start.y - 1 : s->start.y;
                    counter = -1;
                } else {
                    counter = 0;
                    crossedStreet = positionOnMap.front()->name;
                }
            } /* erase street from map */
            else if (counter == -1) {
                IF(positionOnMap.back() && positionOnMap.back()->name == s->name, positionOnMap.pop_back())
                IF(positionOnMap.empty(), positionOnMap.push_back(nullptr))
            } /* add street on map */
            else {
                /* erase nullptr */
                IF(positionOnMap.front() == nullptr, positionOnMap.erase(positionOnMap.begin()))
                positionOnMap.push_back(s);
                /* change background of street */
                Square::layout[x][y]->SetColor("#C0C0C0");
                Square::layout[x][y]->road = true;
            }
        }
    }

    crossedStreet.clear();
    return counter != -1;
}

Street*
StreetMap::GetStreet(const std::string& name)
{
    for (auto &x : Map) {
        for (auto &y : x) {
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

void
StreetMap::UpdateStreet(const std::string& name, float updateSlowdown)
{
    Street *street;
    for (auto &x : Map) {
        for (auto &y : x) {
            if (!y.empty()) {
                if (y.front() && y.front()->name == name) {
                    y.front()->slowdown = updateSlowdown;
                    street = y.front();
                    continue;
                }

                /* two crossed streets */
                if (y.size() > 1 && y.back()->name == name) {
                    y.back()->slowdown = updateSlowdown;
                    street = y.back();
                    continue;
                }
            }
        }
    }

    Square *square;
    /* change street color according to traffic */
    if (street->start.x == street->end.x) {
        for (int i = street->start.y; i <= street->end.y; i++) {
            square = Square::layout[street->start.x][i];
            if (updateSlowdown < 1.33) {
                if (!square->roadBlock)
                    square->SetColor("#C0C0C0");
            }
            else if (updateSlowdown < 1.66) {
                if (!square->roadBlock)
                    square->SetColor("#FFFF66");
            }
            else {
                if (!square->roadBlock)
                    square->SetColor("#FF6600");
            }
        }
    }
    else {
        for (int i = street->start.x; i <= street->end.x; i++) {
            square = Square::layout[i][street->start.y];
            if (updateSlowdown < 1.33) {
                if (!square->roadBlock)
                    square->SetColor("#C0C0C0");
            }
            else if (updateSlowdown < 1.66) {
                if (!square->roadBlock)
                    square->SetColor("#FFFF66");
            }
            else {
                if (!square->roadBlock)
                    square->SetColor("#FF6600");
            }
        }
    }
}

void
StreetMap::AddStreets(const std::string& pathToFile)
{
    QString color;
    Coordinates::Coordinates_S start;
    Coordinates::Coordinates_S end;
    std::ifstream file;
    std::string line;
    std::vector<std::string> tokens;
    bool insert;

    file.open(pathToFile);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    while (std::getline(file, line)) {
        tokens = Functions::Split(line, " ");
        IF(tokens.empty(), break)
        /* coordinates of street */
        start.x = std::stoi(tokens[1]);
        start.y = std::stoi(tokens[2]);
        end.x = std::stoi(tokens[3]);
        end.y = std::stoi(tokens[4]);
        /* add street to map */
        insert = StreetMap::AddStreet(new Street(tokens[0], start, end, 1));
        if (!insert) std::cerr << "Warning: Street " << tokens[0] <<  " cannot be added to map" << std::endl;
    }
    std::vector<std::string>().swap(tokens);
    file.close();
}

void
StreetMap::AddStops(const std::string& pathToFile, QGraphicsScene *scene)
{
    std::ifstream file;
    std::string line, streetName;
    std::vector<std::string> tokens;
    Square *square;
    Street *street = nullptr;
    int x, y;
    int i;

    /* path to image */
    QString path = QString::fromStdString(Functions::GetAbsolutePath("../images/bus_stop.jpeg"));

    file.open(pathToFile);
    IF(!file.is_open(), std::cerr << "Error: Couldn't open file" << std::endl)

    while (std::getline(file, line)) {
        stopData stopInfo;
        /* get content from file */
        streetName = Functions::Split(line, "-")[0];
        tokens = Functions::Split(line, " ");
        /* find street */
        IF(!street || (street && street->name != streetName), street = StreetMap::GetStreet(streetName))
        /* check if street was found and add stop to street */
        if (street) {
            x = std::stoi(tokens[1]);
            y = std::stoi(tokens[2]);

            /* add stop on square */
            square = Square::layout[x][y];
            square->SetColor("#FFFFFF");
            square->hasStop = true;

            Coordinates::Coordinates_S coordinates;
            coordinates.x = x;
            coordinates.y = y;
            Stop *stop = new Stop(tokens[0], coordinates);
            /* add stop to list */
            stopList.emplace(stop, coordinates);

            /* add stop to scene */
            stopInfo.photo = stop->AddStopToScene(scene, path);
            this->stopped.push_back(stopInfo);
        }
        else {
            std::cerr << "Error: Couldn't find street " << streetName << std::endl;
        }

    }
    std::vector<std::string>().swap(tokens);
    file.close();
}

std::map<Stop*, Coordinates::Coordinates_S> StreetMap::stopList;

Coordinates::Coordinates_S
StreetMap::GetStopByName(const std::string& name)
{
    for (const auto& stop : stopList) {
        if (stop.first->stopName == name) {
            return stop.second;
        }
    }

    Coordinates::Coordinates_S coordinates;
    coordinates.x = -1;
    coordinates.y = -1;

    return coordinates;
}

std::string
StreetMap::GetStopByCoordinates(int x, int y)
{
    for (const auto& stop : stopList) {
        if (stop.second.x == x && stop.second.y == y) {
            return stop.first->stopName;
        }
    }

    return "";
}
