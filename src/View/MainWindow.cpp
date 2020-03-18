#include "MainWindow.h"
#include "../../build-src-Desktop-Debug/ui_mainwindow.h" // TODO v qt_creator len ui_mainwindow.h

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* resize main window */
    ResizeWindow();

    CreateScene();
    
    /* view is set to main window */
    //this->setCentralWidget(scene->view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::ResizeWindow()
{
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    width = availableSize.width();
    height = availableSize.height();
    QSize newSize(width * 0.9, height * 0.9);

    setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    newSize,
                    qApp->desktop()->availableGeometry()
            )
    );
}

void
MainWindow::CreateScene()
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setMinimumSize(width * 0.7, 900);
    /* create scene */
    auto *scene = new Scene(scrollArea);
    scrollArea->setWidget(scene);




//    QPushButton *push_Play = new QPushButton(widget);
//    push_Play->setFixedHeight(20);
//    push_Play->setText("Play");
//    connect(push_Play, SIGNAL (released()), scene , SLOT (play()));
//
//    QVBoxLayout *buttonsLayout = new QVBoxLayout();
//    buttonsLayout->addWidget(push_Play);
//    buttonsLayout->setSpacing(10); //space between buttons
//    buttonsLayout->addStretch(100);
}
