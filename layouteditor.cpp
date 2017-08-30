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
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	lef::LEFPort *port;

	QColor color;
	QBrush brush;
	QPen pen;
	mods_t mods = magicdata->getModules();
	foreach (module_info e, mods)
	{
		// fill in library content:
		if(lefdata->isDefinedMacro(e.module_name_plain)) {
			macro = lefdata->getMacro(e.module_name_plain);
			macro->scaleMacro(e.box.width(),e.box.height());
			foreach(pin, macro->getPins()) {
				port = pin->getPort();
				foreach(lef::LEFPortLayer *l, port->getLayers()) {
					color = colorMat(l->getName());
					pen = QPen(color);
					brush = QBrush(color);
					l->setOffsetX(e.xoffset);
					l->setOffsetY(e.yoffset);
					foreach(QRect rect, l->getRects()) {
						editScene->addRect(rect, pen, brush);
					}
				}
			}
		}
		// write layout details:
		pen = QPen(Qt::black);
		editScene->addRect(e.box, pen);
		editScene->addItem(e.instance_name);
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
	QColor mat = QColor("black");

	if (material == "metal1")
		mat = QColor("lightblue");
	if (material == "metal2")
		mat = QColor("blue");
	if (material == "metal3")
		mat = QColor("teal");
	if (material == "metal4")
		mat = QColor("purple");

	if (material == "m1contact")
		mat = QColor("yellow");
	if (material == "m2contact")
		mat = QColor("green");
	if (material == "m3contact")
		mat = QColor("teal");
	if (material == "m4contact")
		mat = QColor("teal");
	
	mat.setAlphaF( 0.5 );

	return mat;
}
