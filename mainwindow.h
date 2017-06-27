#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_MainWindow_destroyed();

    void on_newProject_triggered();
    void on_openMagicFile_triggered();

private:
    Ui::MainWindow *ui;

    rects_t loadMagicFile(QString name);
};

#endif // MAINWINDOW_H
