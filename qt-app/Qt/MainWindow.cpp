#include "MainWindow.h"
#include "../../build-qt-app-Desktop-Debug/ui_mainwindow.h" // TODO v qt_creator len ui_mainwindow.h

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* create scene */
    auto *scene = new Scene();
    scene->CreateMap();
    /* view is set to main window */
    this->setCentralWidget(scene->view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

