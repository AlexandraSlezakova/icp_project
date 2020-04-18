#include "StreetMap.h"

StreetMap::StreetMap(QGraphicsRectItem *parent) : QGraphicsRectItem(parent)
{
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            Map[x][y].push_back(nullptr);
            /* creating square */
            auto *square = new Square();
            /* save its row and col */
            square->row = x;
            square->col = y;
            /* add position on screen */
            square->setPos(x * SQUARE_SIZE, y * SQUARE_SIZE);
            /* set color */
            square->SetColor("#CCFFCC");
            /* save square into layout */
            layout[x][y] = square;
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

    for (int x = s->start->x; x <= s->end->x; x++) {
        for (int y = s->start->y; y <= s->end->y; y++) {
            /* only two streets can cross at a point */
            std::vector<Street *> &positionOnMap = Map[x][y];
            IF(positionOnMap.front() != nullptr && counter >= 0, counter++)

            IF(counter == 1 && crossedStreet.empty(), crossedStreet = positionOnMap.front()->name)

            /* restart for loop */
            if (counter > 1) {
                if (crossedStreet == positionOnMap.front()->name) {
                    x = s->start->y == s->end->y ? s->start->x - 1 : s->start->x;
                    y = s->start->x == s->end->x ? s->start->y - 1 : s->start->y;
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
                layout[x][y]->SetColor("#C0C0C0");
                layout[x][y]->road = true;
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

void
StreetMap::UpdateStreet(const std::string& name, float updateSlowdown)
{
    Street *street;
    for (auto & x : Map) {
        for (auto & y : x) {
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


    /* změna barvy ulice podle provozu */           // Přepsat na ternální operátor aby se to neopakovalo?
    if (street->start->x == street->end->x) {
        for(int i = street->start->y; i <= street->end->y; i++) {
            if( updateSlowdown < 1.33) {
                if (!layout[street->start->x][i]->roadBlock)
                    layout[street->start->x][i]->SetColor("#C0C0C0");
            }
            else if (updateSlowdown < 1.66) {
                if (!layout[street->start->x][i]->roadBlock)
                    layout[street->start->x][i]->SetColor("#FFFF66");
            }
            else {
                if (!layout[street->start->x][i]->roadBlock)
                    layout[street->start->x][i]->SetColor("#FF6600");
            }
        }
    }
    else {
        for(int i = street->start->x; i <= street->end->x; i++) {
            if( updateSlowdown < 1.33) {
                if (!layout[i][street->start->y]->roadBlock)
                    layout[i][street->start->y]->SetColor("#C0C0C0");
            }
            else if (updateSlowdown < 1.66) {
                if (!layout[i][street->start->y]->roadBlock)
                    layout[i][street->start->y]->SetColor("#FFFF66");
            }
            else {
                if (!layout[i][street->start->y]->roadBlock)
                    layout[i][street->start->y]->SetColor("#FF6600");
            }
        }
    }
}

void
StreetMap::AddStreets(const std::string& pathToFile)
{
    QString color;
    Coordinates *start;
    Coordinates *end;
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
        start = new Coordinates(std::stoi(tokens[1]), std::stoi(tokens[2]));
        end = new Coordinates(std::stoi(tokens[3]), std::stoi(tokens[4]));
        /* add street to map */
        insert = StreetMap::AddStreet(new Street(tokens[0], start, end, 1));
        if (!insert) std::cerr << "Warning: Street " << tokens[0] <<  " cannot be added to map" << std::endl;
    }

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
            square = layout[x][y];
            square->SetColor("#FFFFFF");
            square->hasStop = true;

            Stop *stop = new Stop(tokens[0],new Coordinates(x, y));
            /* add stop to list for each street */
            street->SetStop(stop);

            stopInfo.stop = stop;

            /* add stop to scene */
            stopInfo.photo = stop->AddStopToScene(scene, path);
            this->stopped.push_back(stopInfo);
        }
        else {
            std::cerr << "Error: Couldn't find street " << streetName << std::endl;
        }

    }

    file.close();
}
