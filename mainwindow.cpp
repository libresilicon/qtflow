#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "app.h"
#include "environment.h"
#include "lexmagic.h"
#include "magicparser.h"
#include "session.h"
#include "templates.h"
#include "grid.h"
#include "edit.h"
#include "welcome.h"

#include <iostream>
#include <string>

#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QString>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    App(),
    ui(new Ui::MainWindow),
    project(new Qflow),
    dependencies(new Dependencies),
    errorMessage(new QErrorMessage),
    tcsh(new QProcess),
    welcomeWidget(new Welcome),
    editWidget(new Edit),
    buildEnvironment(new Environment),
    qtflowEnvironment(new Environment),
    iopads(new IOPads),
    modules(new Modules),
    session(Session::Instance())
{
    session.setApp(this);
    ui->setupUi(this);
    ui->consoleError->hide();
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->insertTab(0, welcomeWidget, "Welcome");
    ui->tabWidget->insertTab(1, editWidget, "Edit");
    ui->tabWidget->insertTab(2, new QWidget, "Timing");
    ui->tabWidget->insertTab(3, new QWidget, "Design");
    connect(tcsh, SIGNAL(readyReadStandardOutput()), this, SLOT(fireTcsh()));
    connect(tcsh, SIGNAL(readyReadStandardError()), this, SLOT(errorTcsh()));
    connect(tcsh, SIGNAL(finished(int)), this, SLOT(exitTcsh(int)));

    if (!dependencies->tcsh())
        error("tcsh exectuable not found in PATH!");

    if (!dependencies->qflow())
        error("qflow executable not found / wrong qflowprefix: check ~/.qtflowrc");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete errorMessage;
    delete project;
    delete dependencies;
    delete welcomeWidget;
    delete editWidget;
    delete buildEnvironment;
    delete qtflowEnvironment;
    delete iopads;
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_newProject_triggered()
{
    Templates *t = new Templates(this);
    t->show();
}

void MainWindow::on_openProject_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory..."), ".", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (path == QString())
        return;

    QFile project_vars(path + PROJECT_VARS);
    if (!project_vars.exists())
        return;

    session.setProject(path);
    session.getApp()->enableProject();
}

void MainWindow::on_saveFile_triggered()
{
    editWidget->saveFile(session.getFile());
}

void MainWindow::on_openMagicFile_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Magic Files (*.mag)"));

    if (name == QString())
        return;

    qDebug() << "Open magic file:" << name;
    QFile file(name);
    file.open(QFile::ReadOnly);
    QByteArray content(file.readAll());
    MagicParser parser(content);
    rects_t grid = parser.getRectangles();
    Grid *g = new Grid();
    g->show();
    g->RenderRectangles(grid);
}

void MainWindow::on_exit_triggered()
{
    close();
}

void MainWindow::on_buildAll_triggered()
{
    if (tcsh->state() == QProcess::Running)
        return;

    ui->menuSynthesis->setDisabled(true);
    ui->menuPlacement->setDisabled(true);
    ui->menuRouting->setDisabled(true);
    ui->buildAll->setDisabled(true);
    QString path = session.getProject();
    QflowSettings env(path);
    tcsh->setWorkingDirectory(path);
    project->buildAll(tcsh, env.value(DEFAULT_VERILOG));
}

void MainWindow::on_buildEnvironment_triggered()
{
    buildEnvironment->set(new QflowEnvironment(this, session.getProject()));
    buildEnvironment->show();
}

void MainWindow::on_menuSynthesis_triggered()
{
    if (tcsh->state() == QProcess::Running)
        return;

    ui->menuSynthesis->setDisabled(true);
    ui->menuPlacement->setDisabled(true);
    ui->menuRouting->setDisabled(true);
    ui->buildAll->setDisabled(true);
    QString path = session.getProject();
    QflowSettings env(path);
    tcsh->setWorkingDirectory(path);
    project->synthesis(tcsh, env.value(DEFAULT_VERILOG));
}

void MainWindow::on_menuPlacement_triggered()
{
    if (tcsh->state() == QProcess::Running)
        return;

    ui->menuSynthesis->setDisabled(true);
    ui->menuPlacement->setDisabled(true);
    ui->menuRouting->setDisabled(true);
    ui->buildAll->setDisabled(true);
    QString path = session.getProject();
    QflowSettings env(path);
    tcsh->setWorkingDirectory(path);
    project->placement(tcsh, env.value(DEFAULT_VERILOG));
}

void MainWindow::on_menuRouting_triggered()
{
    if (tcsh->state() == QProcess::Running)
        return;

    ui->menuSynthesis->setDisabled(true);
    ui->menuPlacement->setDisabled(true);
    ui->menuRouting->setDisabled(true);
    ui->buildAll->setDisabled(true);
    QString path = session.getProject();
    QflowSettings env(path);
    tcsh->setWorkingDirectory(path);
    project->routing(tcsh, env.value(DEFAULT_VERILOG));
}

void MainWindow::on_menuModules_triggered()
{
    QflowSettings env(session.getProject());
    modules->show();
    modules->refresh(env.value("sourcedir"));
}

void MainWindow::on_menuIOPads_triggered()
{
    iopads->show();
}

void MainWindow::on_menuOptions_triggered()
{
    qtflowEnvironment->set(new QtFlowEnvironment(this));
    qtflowEnvironment->show();
}

void MainWindow::on_mainWelcome_clicked()
{
    ui->tabWidget->show();
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_mainEdit_clicked()
{
    ui->tabWidget->show();
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_tcshExpand_clicked()
{
    ui->tabWidget->hide();
    ui->consoleOut->show();
    ui->consoleError->hide();
}


void MainWindow::on_tcshErrors_clicked()
{
    ui->tabWidget->hide();
    ui->consoleOut->hide();
    ui->consoleError->show();
}

void MainWindow::fireTcsh()
{
    QByteArray bytes = tcsh->read(4096);
    ui->consoleOut->insertPlainText(bytes);
    ui->consoleOut->verticalScrollBar()->setValue(ui->consoleOut->verticalScrollBar()->maximum());
}

void MainWindow::errorTcsh()
{
    QByteArray bytes = tcsh->readAllStandardError();
    ui->consoleError->insertPlainText(bytes);
    ui->consoleError->verticalScrollBar()->setValue(ui->consoleError->verticalScrollBar()->maximum());
}

void MainWindow::exitTcsh(int code)
{
    qDebug() << "Tcsh exited with code" << code;
    ui->menuSynthesis->setDisabled(false);
    ui->menuPlacement->setDisabled(false);
    ui->menuRouting->setDisabled(false);
    ui->buildAll->setDisabled(false);
}

void MainWindow::enableProject()
{
    editWidget->loadProject(session.getProject());
    ui->tabWidget->show();

    ui->buildAll->setDisabled(false);
    ui->buildSteps->setDisabled(false);
    ui->buildEnvironment->setDisabled(false);
    ui->menuSynthesis->setDisabled(false);
    ui->menuPlacement->setDisabled(false);
    ui->menuRouting->setDisabled(false);
    ui->menuModules->setDisabled(false);
    ui->mainEdit->setDisabled(false);
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::disableProject()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->show();

    ui->buildAll->setDisabled(true);
    ui->buildSteps->setDisabled(true);
    ui->buildEnvironment->setDisabled(true);
    ui->menuSynthesis->setDisabled(true);
    ui->menuPlacement->setDisabled(true);
    ui->menuRouting->setDisabled(true);
    ui->menuModules->setDisabled(true);
    ui->mainEdit->setDisabled(true);
    ui->mainTiming->setDisabled(true);
    ui->mainDesign->setDisabled(true);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::enableFile()
{
    ui->saveFile->setDisabled(false);
}

void MainWindow::disableFile()
{
    ui->saveFile->setDisabled(true);
}

void MainWindow::error(QString e)
{
    errorMessage->showMessage(e);
    errorMessage->show();
}
