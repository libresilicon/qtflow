#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app.h"
#include "project.h"
#include "session.h"
#include "common.h"
#include "edit.h"
#include "environment.h"
#include "dependencies.h"
#include "modules.h"
#include "iopads.h"
#include "welcome.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QErrorMessage>

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

    void enableFile();
    void disableFile();

    void error(QString);

public slots:
    void fireTcsh();
    void errorTcsh();
    void exitTcsh(int);

private slots:
    void on_MainWindow_destroyed();

    void on_newProject_triggered();
    void on_openProject_triggered();

    void on_openMagicFile_triggered();

    void on_saveFile_triggered();

    void on_exit_triggered();

    void on_buildAll_triggered();
    void on_buildEnvironment_triggered();
    void on_menuModules_triggered();
    void on_menuSynthesis_triggered();
    void on_menuPlacement_triggered();
    void on_menuRouting_triggered();

    void on_menuIOPads_triggered();
    void on_menuOptions_triggered();

    void on_mainWelcome_clicked();
    void on_mainEdit_clicked();

    void on_tcshExpand_clicked();

    void on_tcshErrors_clicked();

    void on_toolRefresh_triggered();

private:
    Ui::MainWindow *ui;
    IProject *project;
    IDependencies *dependencies;
    QErrorMessage *errorMessage;
    QProcess *tcsh;
    Welcome *welcomeWidget;
    Edit *editWidget;
    Environment *buildEnvironment;
    Environment *qtflowEnvironment;
    IOPads *iopads;
    Modules *modules;

    Session &session;
};

#endif // MAINWINDOW_H
