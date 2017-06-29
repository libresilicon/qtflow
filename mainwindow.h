#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app.h"
#include "project.h"
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

public slots:
    void fireTcsh();
    void exitTcsh(int);

private slots:
    void on_MainWindow_destroyed();

    void on_newProject_triggered();
    void on_openProject_triggered();

    void on_openMagicFile_triggered();

    void on_exit_triggered();

    void on_buildAll_triggered();

private:
    Ui::MainWindow *ui;
    IProject *project;
    QProcess *tcsh;

    Session &session;
};

#endif // MAINWINDOW_H
