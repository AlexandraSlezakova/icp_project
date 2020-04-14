/**
 * @file MainWindow.h
 * @brief MainWindow header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Scene.h"

#define TIME_AREA_WIDTH     100
#define TIME_AREA_HEIGHT    45

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
     * @param scene
     */
    void InitButtons(QWidget *parent, Scene *scene);

    void InitSliders(QWidget *parent, Scene *scene);

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
     */
    void ResetTimer();

    void value(int slowDown);



protected:
    /**
     * @brief display current time
     * @param event
     */
    void timerEvent(QTimerEvent *event) override;

private:
    QComboBox *combobox;
    QSlider *slider;
    std::string streetUpdate;           //!<

    QPushButton *timerButton;       //!< button to start or stop timer
    QPushButton *zoomButtonAdd;     //!< button to zoom in scene
    QPushButton *zoomButtonSub;     //!< button to zoom out scene
    QPlainTextEdit *timeArea;       //!< textarea with time
    QString previousTime;           //!< time before change
    Ui::MainWindow *ui;
    Scene *scene;

    int timerId = 0;
    int width = 0;          //!< width of screen
    int height = 0;         //!< height of screen
    int stopFlag = 0;       //!< timer is active or inactive

};
#endif // MAINWINDOW_H
