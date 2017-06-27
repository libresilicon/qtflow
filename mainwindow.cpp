#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lexmagic.h"
#include "magicparser.h"
#include "templates.h"
#include "grid.h"

#include <iostream>
#include <string>

#include <QFileDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_newProject_triggered()
{
    Templates *t = new Templates();
    t->show();
}

void MainWindow::on_openMagicFile_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Magic Files (*.mag)"));
    std::cout << "Open file: " << name.toStdString() << std::endl;
    rects_t grid = loadMagicFile(name);
    Grid *g = new Grid();
    g->show();
    g->RenderRectangles(grid);
    return;
}

void MainWindow::on_exit_triggered()
{
    this->close();
}

rects_t MainWindow::loadMagicFile(QString name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);
    QByteArray content(file.readAll());
    MagicParser parser(content);

    return parser.getRectangles();
}
