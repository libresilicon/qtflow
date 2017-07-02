#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "app.h"
#include "buildenvironment.h"
#include "buildsteps.h"
#include "lexmagic.h"
#include "magicparser.h"
#include "session.h"
#include "templates.h"
#include "grid.h"
#include "mainedit.h"
#include "mainprojects.h"

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
    project(new Project),
    tcsh(NULL),
    projectsWidget(new MainProjects),
    editWidget(new MainEdit),
    session(Session::Instance())
{
    session.setApp(this);
    ui->setupUi(this);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->insertTab(0, projectsWidget, "Projects");
    ui->tabWidget->insertTab(1, editWidget, "Edit");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete project;
    delete projectsWidget;
    delete editWidget;
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
    qDebug() << "Open file:" << name;
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
    if (tcsh != NULL)
        return;

    QString path = session.getProject();
    ProjectSettings environment(path);
    tcsh = new QProcess(this);
    tcsh->setWorkingDirectory(path);
    project->executeQflow(tcsh);
    connect(tcsh, SIGNAL(readyRead()), this, SLOT(fireTcsh()));
    connect(tcsh, SIGNAL(finished(int)), this, SLOT(exitTcsh(int)));
}

void MainWindow::on_buildSteps_triggered()
{
    BuildSteps *w = new BuildSteps();
    w->show();
}

void MainWindow::on_buildEnvironment_triggered()
{
    BuildEnvironment *w = new BuildEnvironment();
    w->show();
}

void MainWindow::on_mainProjects_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_mainEdit_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::fireTcsh()
{
    QByteArray bytes = tcsh->read(4096);
    ui->consoleOut->insertPlainText(bytes);
    ui->consoleOut->verticalScrollBar()->setValue(ui->consoleOut->verticalScrollBar()->maximum());
}

void MainWindow::exitTcsh(int code)
{
    qDebug() << "Tcsh exited with code" << code;
    delete tcsh;
    tcsh = NULL;
}

void MainWindow::enableProject()
{
    ui->buildAll->setDisabled(false);
    ui->quickBuild->setDisabled(false);
    ui->buildSteps->setDisabled(false);
    ui->buildEnvironment->setDisabled(false);

    ui->mainEdit->setDisabled(false);
    editWidget->loadProject(session.getProject());
}

void MainWindow::disableProject()
{
    ui->buildAll->setDisabled(true);
    ui->quickBuild->setDisabled(true);
    ui->buildSteps->setDisabled(true);
    ui->buildEnvironment->setDisabled(true);

    ui->mainEdit->setDisabled(true);
}

void MainWindow::enableFile()
{
    ui->saveFile->setDisabled(false);
}

void MainWindow::disableFile()
{
    ui->saveFile->setDisabled(true);
}
