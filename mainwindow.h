#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app.h"
#include "session.h"
#include "common.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public App
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void enableProject();
    void disableProject();

private slots:
    void on_MainWindow_destroyed();

    void on_newProject_triggered();

    void on_openMagicFile_triggered();

    void on_exit_triggered();

private:
    Ui::MainWindow *ui;

    Session &session;

    rects_t loadMagicFile(QString name);
};

#endif // MAINWINDOW_H
