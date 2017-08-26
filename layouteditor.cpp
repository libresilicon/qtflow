#include "layouteditor.h"
#include <QAbstractScrollArea>

LayoutEditor::LayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(new magic::MagicData()),
	filePath(QString()),
	editScene(new QGraphicsScene(this))
{
	setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
	//setBaseSize(parent->maximumWidth(),parent->maximumHeight());
	//editScene->setBackgroundBrush(Qt::blue);
	editScene->setBackgroundBrush(Qt::white);
	//setBaseSize(300,300);
	//editScene->setSceneRect(0,0,300,300);
	setScene(editScene);
}

void LayoutEditor::mousePressEvent(QMouseEvent * e)
{
	double rad = 10;
	QPointF pt = mapToScene(e->pos());
	editScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
	QTextStream(stdout) << "Drawing here " << "\t x:" << pt.x() << "\t y:" << pt.y()  << '\n';
}

void LayoutEditor::drawBoxes()
{
	QColor color;
	rects_t layer;
	layer_rects_t layers = magicdata->getBoxes();
	foreach(QString layerN, layers.keys()) {
		color = colorMat(layerN);
		layer = layers[layerN];
		foreach (const QRect& e, layer)
		{
			QPen pen = QPen(color);
			QTextStream(stdout) << "Coming to here " << "\t x:" << e.x() << "\t y:" << e.y()  << '\n';
			QTextStream(stdout) << "Scene rect " << "\t x:" << editScene->sceneRect().x() << "\t y:" << editScene->sceneRect().y()  << '\n';
			editScene->addRect(e, pen);
		}
	}
}

void LayoutEditor::drawRectangles()
{
	QColor color;
	rects_t layer;
	layer_rects_t layers = magicdata->getRectangles();
	foreach(QString layerN, layers.keys()) {
		color = colorMat(layerN);
		layer = layers[layerN];
		foreach (const QRect& e, layer)
		{
			QPen pen = QPen(color);
			QBrush brush = QBrush(color);
			QTextStream(stdout) << "Coming to here " << "\t x:" << e.x() << "\t y:" << e.y()  << '\n';
			QTextStream(stdout) << "Scene rect " << "\t x:" << editScene->sceneRect().x() << "\t y:" << editScene->sceneRect().y()  << '\n';
			editScene->addRect(e, pen, brush);
		}
	}
}

LayoutEditor::~LayoutEditor()
{
}

/*void LayoutEditor::resizeEvent(QResizeEvent*)
{
	setBaseSize(this->maximumWidth(),this->maximumHeight());
	drawBoxes();
}*/

void LayoutEditor::loadFile(QString file)
{
	filePath = file;
	magicdata->loadFile(file);
	drawBoxes();
	drawRectangles();
	//fitInView(editScene->sceneRect(), Qt::KeepAspectRatio);
}

void LayoutEditor::saveFile()
{
}

QString LayoutEditor::getFilePath()
{
	return filePath;
}

bool LayoutEditor::changes()
{
	return false;
}

QColor LayoutEditor::colorMat(QString material)
{
	// TODO:
	// make this configuration based!
	// don't hardcode this!

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
