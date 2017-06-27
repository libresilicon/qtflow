#include <QtGui/QMainWindow>

#include "layout.h"
#include "ui_layout.h"

#include "lexmagic.h"
//#include "parser.h"
#include "magicparser.h"
#include "grid.h"

#include <iostream>
#include <string>

#include <QFileDialog>
#include <QString>

Layout::Layout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Layout)
{
    ui->setupUi(this);
}

Layout::~Layout()
{
    delete ui;
}

void Layout::on_Layout_destroyed()
{

}

void Layout::on_openFile_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Magic Files (*.mag)"));
    std::cout << "Open file: " << name.toStdString() << std::endl;
    rects_t grid = loadMagicFile(name);
    Grid *g = new Grid();
    g->show();
    g->RenderRectangles(grid);
    return;
}

rects_t Layout::loadMagicFile(QString name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);
    QByteArray content(file.readAll());
    MagicParser parser(content);

    return parser.getRectangles();
}
