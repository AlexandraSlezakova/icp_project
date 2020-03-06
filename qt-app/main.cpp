#include "Qt/mainwindow.h"
#include "main.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    std::string streetName;

    auto map = new StreetMap();
    map->AddStreets("/home/alexandra/Projects/icp-project/ulice.txt");
    map->AddStops("/home/alexandra/Projects/icp-project/zastavky.txt");

    QApplication a(argc, argv);

    MainWindow window;
    window.setWindowTitle("ICP 2020");
    window.show();

    return a.exec();
}
