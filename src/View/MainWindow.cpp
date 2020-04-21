#include "MainWindow.h"
//#include "../../build-src-Desktop-Debug/ui_mainwindow.h" // Cesta Alex  TODO v qt_creator len ui_mainwindow.h
#include "../../build-src-Desktop-Ladu011bnu00ed/ui_mainwindow.h" // Cesta Martin
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ResizeWindow();
    CreateScene();
    timerInterval = 1000;
    timerId = startTimer(timerInterval);
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

    /* text area with timetable */
    Bus::InitTimetableArea(widget, width, height);
    /* text area with time */
    InitTimeArea(widget);
    /* buttons */
    InitButtons(widget);
    /* slider */
    InitSliders(widget);
}

void
MainWindow::timerEvent(QTimerEvent *event)
{
    timeArea->clear();
    timeArea->appendPlainText(Timer::GetTime());

    int minute = Timer::GetMinute();
    int second = Timer::GetSecond();
    static int iteration = 0;
    /* every five minutes create new buses */
    if (minute > 0 && !(minute % 10) && !second) {
        iteration++;
        scene->AddBuses(iteration);
    }
    scene->MoveBuses();
}

void
MainWindow::InitTimeArea(QWidget *parent)
{
    QFont font;
    font.setPointSize(23);
    /* text area with time */
    timeArea = new QPlainTextEdit(parent);
    timeArea->setFixedSize(TIME_AREA_WIDTH, TIME_AREA_HEIGHT);
    timeArea->move(5, 5);
    timeArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    timeArea->setFont(font);

    /* button for stopping and starting time */
    timerButton = new QPushButton(parent);
    timerButton->move(TIME_AREA_WIDTH + 10, 5);
    timerButton->setFixedSize(90, 30);
    timerButton->setText("Change time");
    connect(timerButton, SIGNAL (released()), this , SLOT(StopTimer()));

    /* reset button */
    QPushButton *resetButton = new QPushButton(parent);
    resetButton->move(TIME_AREA_WIDTH + 100, 5);
    resetButton->setFixedSize(90, 30);
    resetButton->setText("Reset timer");
    connect(resetButton, SIGNAL (released()), this , SLOT(ResetTimer()));

    /* change interval of timer button */
    timerLabel = new QLabel(parent);
    timerLabel->move(TIME_AREA_WIDTH + 10, 35);
    timerLabel->setFixedSize(150,40);
    timerLabel->setText("Timer interval = " + QString::number(100) + "%");

    QPushButton *timerButtonAdd = new QPushButton(parent);
    timerButtonAdd->move(TIME_AREA_WIDTH + 160, 40);
    timerButtonAdd->setFixedSize(30, 30);
    timerButtonAdd->setText("+");
    connect(timerButtonAdd, SIGNAL (released()), this, SLOT(TimerPlus()));

    QPushButton *timerButtonSub = new QPushButton(parent);
    timerButtonSub->move(TIME_AREA_WIDTH + 190, 40);
    timerButtonSub->setFixedSize(30, 30);
    timerButtonSub->setText("-");
    connect(timerButtonSub, SIGNAL (released()), this, SLOT(TimerSub()));
}

void
MainWindow::InitButtons(QWidget *parent)
{
    QWidget *buttons = new QWidget(parent);
    buttons->move(0, TIME_AREA_HEIGHT);

    /* zoom buttons */
    QPushButton *zoomButtonAdd = new QPushButton(parent);
    zoomButtonAdd->move(TIME_AREA_WIDTH, 770);
    zoomButtonAdd->setFixedSize(30, 30);
    zoomButtonAdd->setText("+");
    connect(zoomButtonAdd, SIGNAL (released()), scene, SLOT(ZoomAdd()));

    QPushButton *zoomButtonSub = new QPushButton(parent);
    zoomButtonSub->move(TIME_AREA_WIDTH + 30, 770);
    zoomButtonSub->setFixedSize(30, 30);
    zoomButtonSub->setText("-");
    connect(zoomButtonSub, SIGNAL (released()), scene, SLOT(ZoomSub()));

    /* print numb. expression of zoom */
    scene->zoomText = new QLabel(parent);
    scene->zoomText->move(5, 770);
    scene->zoomText->setFixedSize(150,30);
    scene->zoomText->setText("Actual zoom = " + QString::number(scene->zoom_act,'f',2));

    roadBlockButton = new QPushButton(parent);
    roadBlockButton->move(5, 725);
    roadBlockButton->setFixedSize(200, 30);
    roadBlockButton->setText("RoadBlockMode OFF");
    roadBlockButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    connect(roadBlockButton, SIGNAL (released()), this, SLOT(RoadBlockSwitcher()));
}

void MainWindow::InitSliders(QWidget *parent) {

    std::ifstream file;
    std::string line;
    std::vector<std::string> street;

    /* streetpicker */
    combobox = new QComboBox(parent);
    combobox->move(5,820);
    combobox->setFixedSize(150, 40);

    file.open(Functions::GetAbsolutePath("../files/ulice.txt"));

    /* add choice to streetpicker */
    while (std::getline(file, line)) {
        street = Functions::Split(line, " ");
        combobox->addItem(QString::fromStdString(street[0]));
    }

    /* slider for changing street slowdown */
    QSlider *slider = new QSlider(Qt::Horizontal,parent);
    slider->move(160, 827);
    slider->setFixedSize(80, 40);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(ChangedSlowDownValue(int)));
}

void
MainWindow::StopTimer()
{
    if (!stopFlag) {
        killTimer(timerId);
        timerButton->setText("Start timer");
        stopFlag = 1;
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
        Timer::ChangeTime(std::stoi(time[0]), std::stoi(time[1]), std::stoi(time[2]));
    }
}

void
MainWindow::ResetTimer()
{
    Timer::ResetTime();
    timerInterval = 1000;
    timerLabel->setText("Timer interval = " + QString::number(100) + "%");
}


void 
MainWindow::ChangedSlowDownValue(int slowDown) 
{
    scene->map->UpdateStreet(combobox->currentText().toStdString(),(float)slowDown / 100 + 1);
}

void
MainWindow::RoadBlockSwitcher()
{
    if (!scene->roadBlockMode) {
        roadBlockButton->setText("RoadBlockMode ON");
        roadBlockButton->setStyleSheet("background-color: green; color: white; font-weight: bold;");
        scene->roadBlockMode = true;
    }
    else {
        roadBlockButton->setText("RoadBlockMode OFF");
        roadBlockButton->setStyleSheet("background-color: red; color: white; font-weight: bold;");
        scene->roadBlockMode = false;
        scene->checkRoadBlockBus();
    }
}

void
MainWindow::TimerPlus()
{
    /* value of interval cannot be negative integer */
    if (timerInterval > 0) {
        timerInterval = timerInterval <= 200 ? timerInterval - 25 : timerInterval - 200;
    }
    killTimer(timerId);
    timerId = startTimer(timerInterval);
    int percentage = (timerInterval * 100) / 1000;
    percentage = 100 + (100 - percentage);
    QString warning;
    IF(!timerInterval, warning = "\nWarning: maximum speed")
    timerLabel->setText("Timer interval = " + QString::number(percentage) + "%" + warning);
}

void
MainWindow::TimerSub()
{
    killTimer(timerId);
    timerInterval += 200;
    timerId = startTimer(timerInterval);
    int percentage = ((timerInterval * 100) / 1000) - 100;
    percentage = 100 - percentage;
    timerLabel->setText("Timer interval = " + QString::number(percentage) + "%");
}

