#include "grid.h"
#include "ui_grid.h"

#include <iostream>
#include <string>

#include <QGraphicsScene>
#include <QGraphicsView>

Grid::Grid(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Grid),
	scene(new QGraphicsScene)
{
	ui->setupUi(this);
	ui->graphicsView->setScene(scene);
}

Grid::~Grid()
{
	delete scene;
	delete ui;
}

void Grid::RenderRectangles(QString layer, rects_t rectangles)
{
	foreach (const QRect& e, rectangles)
	{
		QColor color = colorMat(layer);
		QPen pen = QPen(color);
		QBrush brush = QBrush(color);
		scene->addRect(e, pen, brush);
	}
}

QColor Grid::colorMat(QString m)
{
	// TODO:
	// make this configuration based!
	// don't hardcode this!

    std::string material = m.toStdString();
    if (material == "metal1")
        return QColor("lightblue");
    if (material == "metal2")
        return QColor("blue");
    if (material == "metal3")
        return QColor("teal");
    if (material == "metal4")
        return QColor("purple");

    if (material == "m1contact")
        return QColor("yellow");
    if (material == "m2contact")
        return QColor("green");
    if (material == "m3contact")
        return QColor("teal");
    if (material == "m4contact")
        return QColor("teal");

    return QColor("black");
}
