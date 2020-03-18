#include "MainWindow.h"
#include "../../build-qt-app-Desktop-Debug/ui_mainwindow.h" // TODO v qt_creator len ui_mainwindow.h

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* create scene */
    auto *scene = new Scene();
    scene->CreateMap();
    /* resize main window */
    ResizeWindow();
    
    /* view is set to main window */
    this->setCentralWidget(scene->view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::ResizeWindow()
{
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    int width = availableSize.width();
    int height = availableSize.height();
    width *= 0.9;
    height *= 0.9;
    QSize newSize(width, height );

    setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    newSize,
                    qApp->desktop()->availableGeometry()
            )
    );
}
