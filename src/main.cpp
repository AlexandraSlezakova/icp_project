#include "main.h"

int main(int argc, char *argv[])
{
    std::string streetName;

    QApplication a(argc, argv);
    MainWindow window;
    window.setWindowTitle("ICP 2020");
    window.show();

    return a.exec();
}
