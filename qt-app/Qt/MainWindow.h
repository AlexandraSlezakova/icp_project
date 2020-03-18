/**
 * @file MainWindow.h
 * @brief MainWindow header file
 * @author Martin Vadura, Alexandra Slezakova
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Scene.h"

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
