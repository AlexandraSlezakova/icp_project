#include "MainWindow.h"
//#include "../../build-src-Desktop-Debug/ui_mainwindow.h" // Cesta Alex  TODO v qt_creator len ui_mainwindow.h
#include "../../build-src-Desktop-Ladu011bnu00ed/ui_mainwindow.h" // Cesta Martin
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timerId = startTimer(1000);
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
    scene = new Scene(scrollArea);
    scrollArea->setWidget(scene);

    QWidget *widget = new QWidget(this);
    /* move widget */
    widget->move(width * 0.7, 0);
    widget->setMinimumSize(width * 0.3, height * 0.9);

    /* text area for bus timetable */
    scene->text = new QPlainTextEdit(widget);
    scene->text->setMinimumSize(width * 0.3, height * 0.6);
    scene->text->move(0, TIME_AREA_HEIGHT * 2);

    /* text area with time */
    InitTimeArea(widget);


    /* buttons */
    InitButtons(widget, scene);

    InitSliders(widget,scene);
}


// autobuse se vymažua a npíšse se jinde, timer na to obnovení
void
MainWindow::timerEvent(QTimerEvent *event)
{
    timeArea->clear();
    timeArea->appendPlainText(Timer::GetTime());
}

void
MainWindow::InitTimeArea(QWidget *parent)
{
    QFont font;
    font.setPointSize(16);
    /* text area with time */
    timeArea = new QPlainTextEdit(parent);
    timeArea->setFixedSize(TIME_AREA_WIDTH, TIME_AREA_HEIGHT);
    timeArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    timeArea->setFont(font);
    /* button for stopping and starting time */
    timerButton = new QPushButton(parent);
    timerButton->move(TIME_AREA_WIDTH + 5, 0);
    timerButton->setFixedSize(90, 30);
    timerButton->setText("Change time");
    connect(timerButton, SIGNAL (released()), this , SLOT(StopTimer()));
    /* reset button */
    QPushButton *resetButton = new QPushButton(parent);
    resetButton->move(TIME_AREA_WIDTH + 95, 0);
    resetButton->setFixedSize(90, 30);
    resetButton->setText("Reset timer");
    connect(resetButton, SIGNAL (released()), this , SLOT(ResetTimer()));

}

void
MainWindow::InitButtons(QWidget *parent, Scene *scene)
{
    QWidget *buttons = new QWidget(parent);
    buttons->move(0, TIME_AREA_HEIGHT);
    /* button for bus 1 */
    QPushButton *bus1 = new QPushButton(buttons);
    bus1->setFixedSize(140, 30);
    bus1->setText("Timetable of bus #1");
    connect(bus1, SIGNAL (released()), scene , SLOT(GetBus1Timetable()));
    scene->busId++;

    /*Zooms buttons */
    zoomButtonAdd = new QPushButton(parent);
    zoomButtonAdd->move(TIME_AREA_WIDTH + 70, 915);
    zoomButtonAdd->setFixedSize(30, 30);
    zoomButtonAdd->setText("+");
    connect(zoomButtonAdd, SIGNAL (released()), scene, SLOT(ZoomAdd()));


    zoomButtonSub = new QPushButton(parent);
    zoomButtonSub->move(TIME_AREA_WIDTH + 100, 915);
    zoomButtonSub->setFixedSize(30, 30);
    zoomButtonSub->setText("-");
    connect(zoomButtonSub, SIGNAL (released()), scene, SLOT(ZoomSub()));

    /* Print numb. expression of zoom */
    scene->zoomText = new QLabel(parent);
    scene->zoomText->move(TIME_AREA_WIDTH-80, 915);
    scene->zoomText->setFixedSize(150,30);
    scene->zoomText->setText("Actual zoom = " + QString::number(scene->zoom_act,'f',2));
    scene->zoomText->show();


}

void MainWindow::InitSliders(QWidget *parent, Scene *scene) {

    std::ifstream file;
    std::string line;
    std::vector<std::string> street;

    combobox = new QComboBox(parent);
    combobox->move(TIME_AREA_WIDTH + 20, 815);
    combobox->setFixedSize(60,40);

    file.open(Functions::GetAbsolutePath("../files/ulice.txt"));

    while (std::getline(file, line)) {

        street = Functions::Split(line, " ");
        combobox->addItem(QString::fromStdString(street[0]));
    }



    slider = new QSlider(Qt::Horizontal,parent);
    slider->move(TIME_AREA_WIDTH + 70, 815);
    slider->setFixedSize(60,40);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));
}

void
MainWindow::StopTimer()
{
    if (!stopFlag) {
        killTimer(timerId);
        timerButton->setText("Start timer");
        stopFlag = 1;
        previousTime = timeArea->toPlainText();
        connect(timeArea, SIGNAL(textChanged()), this, SLOT(ReadInput()));
    } else {
        timerButton->setText("Change time");
        stopFlag = 0;
        timerId = startTimer(1000);
        disconnect(timeArea, SIGNAL(textChanged()), this, SLOT(ReadInput()));
    }
}

void
MainWindow::ReadInput()
{
    /* get time from text area */
    QString data = timeArea->toPlainText();
    std::vector<std::string> time = Functions::Split(data.toStdString(), ":");
    if (time.size() == 3) {
        Timer::ChangeTime(std::stoi(time[0]), std::stoi(time[1]), std::stoi(time[2]),
                previousTime);
    }
}

void
MainWindow::ResetTimer()
{
    Timer::ResetTime();
}

void MainWindow::value(int slowDown) {

    scene->map->UpdateStreet(combobox->currentText().toStdString(),(float)slowDown / 100 + 1);

}




