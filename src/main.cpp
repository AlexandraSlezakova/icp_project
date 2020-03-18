#include "main.h"

int main(int argc, char *argv[])
{
    std::string streetName;

    QApplication application(argc, argv);
    MainWindow window;
    window.setWindowTitle("ICP 2020");
    window.show();

    return application.exec();
}
