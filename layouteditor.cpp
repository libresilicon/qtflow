#include "layouteditor.h"
#include <QAbstractScrollArea>

#include "magicdata.h"
#include "lefdata.h"

LayoutEditor::LayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(new magic::MagicData()),
	filePath(QString()),
	editScene(new QGraphicsScene(this))
{
	editScene->setBackgroundBrush(Qt::white);
	setScene(editScene);
}

void LayoutEditor::mousePressEvent(QMouseEvent * e)
{
	double rad = 10;
	QPointF pt = mapToScene(e->pos());
	editScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
	QTextStream(stdout) << "Drawing here " << "\t x:" << pt.x() << "\t y:" << pt.y()  << '\n';
}

void LayoutEditor::drawRectangles()
{
	QColor color;
	rects_t layer;
	layer_rects_t layers = magicdata->getRectangles();
	foreach(QString layerN, layers.keys()) {
		color = colorMat(layerN);
		layer = layers[layerN];
		QPen pen = QPen(color);
		QBrush brush = QBrush(color);
		foreach (const QRect& e, layer)
		{
			editScene->addRect(e, pen, brush);
		}
	}
}

void LayoutEditor::drawModuleInfo()
{
	QColor color;
	QBrush brush;
	QPen pen = QPen(Qt::black);
	mods_t mods = magicdata->getModules();
	foreach (module_info e, mods)
	{
		editScene->addRect(e.box, pen);
		editScene->addItem(e.instance_name);
		if(lefdata->isDefinedMacro(e.module_name_plain)) {
			foreach(QString name, lefdata->getMacro(e.module_name_plain).getPinNames()) {
				foreach(QString l, lefdata->getMacro(e.module_name_plain).getPin(name).getPortLayers()) {
					color = colorMat(l);
					pen = QPen(color);
					brush = QBrush(color);
					foreach(QRect r, lefdata->getMacro(e.module_name_plain).getPin(name).getPortRectangles(l)) {
						editScene->addRect(r, pen, brush);
					}
				}
			}
		}
		//editScene->addItem(e.module_name);
	}
}

void LayoutEditor::loadFile(QString file)
{
	filePath = file;
	magicdata->loadFile(file);
	magicdata->getTechnology(); // TODO: do something with this here
	lefdata = new lef::LEFData("/usr/share/qflow/tech/osu035/osu035_stdcells.lef");
	drawRectangles();
	drawModuleInfo();
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
