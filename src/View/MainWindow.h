/**
 * @file MainWindow.h
 * @brief MainWindow header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Scene.h"

#define TIME_AREA_WIDTH     140
#define TIME_AREA_HEIGHT    60

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief main Window constructor
     * @param parent parent widget
     */
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    /**
     * @brief set window to 90% of screen
     */
    void ResizeWindow();

    /**
     * @brief create whole scene with map, buttons and textarea
     */
    void CreateScene();

    /**
     * @brief initialize area for time
     * @param parent parent widget
     */
    void InitTimeArea(QWidget *parent);

    /**
     * @brief initialize all buttons
     * @param parent parent widget
     */
    void InitButtons(QWidget *parent);

    /**
     * @brief initialize slider
     * @param parent parent widget
     */
    void InitSliders(QWidget *parent);

public slots:
    /**
     * @bried stop timer to read input
     */
    void StopTimer();

    /**
     * @brief read input from text are with time
     */
    void ReadInput();

    /**
     * @brief reset timer
     * and set interval to default value
     */
    void ResetTimer();


    void ChangedSlowDownValue(int slowDown);


    void RoadBlockSwitcher();

    /**
     * @brief decrease timer interval
     * the time goes faster
     */
    void TimerPlus();

    /**
     * @breif increase timer interval
     * the time does slower
     */
    void TimerSub();

protected:
    /**
     * @brief display current time
     * @param event
     */
    void timerEvent(QTimerEvent *event) override;

private:
    QComboBox *combobox;            //!< combobox on pick street to slowdown
    std::string streetUpdate;       //!< street name
    QPushButton *roadBlockButton;   //!< button to add roadblock
    QPushButton *timerButton;       //!< button to start or stop timer
    QPlainTextEdit *timeArea;       //!< textarea with time
    QLabel *timerLabel;
    Ui::MainWindow *ui;
    Scene *scene;

    int timerId = 0;
    int timerInterval = 0;
    int width = 0;                  //!< width of screen
    int height = 0;                 //!< height of screen
    int stopFlag = 0;               //!< timer is active or inactive
};
#endif // MAINWINDOW_H
