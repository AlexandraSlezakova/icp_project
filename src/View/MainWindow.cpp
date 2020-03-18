#include "MainWindow.h"
#include "../../build-src-Desktop-Debug/ui_mainwindow.h" // TODO v qt_creator len ui_mainwindow.h

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ResizeWindow();
    CreateScene();
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
    scrollArea->setMinimumSize(width * 0.7, height * 0.9);
    /* create scene */
    auto *scene = new Scene(scrollArea);
    scrollArea->setWidget(scene);

    QWidget *widget = new QWidget(this);
    /* move widget */
    widget->move(width * 0.7, 0);
    widget->setMinimumSize(width * 0.3, height * 0.9);

    /* text area for bus timetable */
    scene->text = new QPlainTextEdit(widget);
    scene->text->setMinimumSize(width * 0.3, height * 0.8);
    scene->text->move(0, 30);

    QWidget *buttons = new QWidget(widget);

    /* button for bus 1 */
    QPushButton *bus1 = new QPushButton(buttons);
    bus1->setFixedHeight(30);
    bus1->setFixedWidth(140);
    bus1->setText("Timetable of bus #1");
    connect(bus1, SIGNAL (released()), scene , SLOT(GetBus1Timetable()));
}