#include "MainWindow.h"
#include "../../build-qt-app-Desktop-Debug/ui_mainwindow.h" // TODO v qt_creator len ui_mainwindow.h

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    //this->setStyleSheet("background-color: pink;");
    auto *scene = new Scene();
    scene->CreateMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

