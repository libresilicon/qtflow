#include "deflayouteditor.h"
#include <QAbstractScrollArea>

#include "defdata.h"
#include "../lef/lefdata.h"

DEFLayoutEditor::DEFLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	defdata(NULL),
	lefdata(NULL),
	filePath(QString()),
	editScene(new QGraphicsScene(this))
{
	editScene->setBackgroundBrush(Qt::white);
	setScene(editScene);
}

void DEFLayoutEditor::mousePressEvent(QMouseEvent * e)
{
	double rad = 10;
	QPointF pt = mapToScene(e->pos());
	editScene->addEllipse(pt.x()-rad, pt.y()-rad, rad*2.0, rad*2.0, QPen(), QBrush(Qt::SolidPattern));
	QTextStream(stdout) << "Drawing here " << "\t x:" << pt.x() << "\t y:" << pt.y()  << '\n';
}

void DEFLayoutEditor::loadFile(QString file)
{
	filePath = file;
	if(defdata) delete defdata;
	defdata = new def::DEFData(file);
	if(defdata) delete defdata;
	lefdata = new lef::LEFData("/usr/share/qflow/tech/osu035/osu035_stdcells.lef");
	//drawRectangles();
	//drawModuleInfo();
}

void DEFLayoutEditor::saveFile()
{
}

QString DEFLayoutEditor::getFilePath()
{
	return filePath;
}

bool DEFLayoutEditor::changes()
{
	return false;
}

QColor DEFLayoutEditor::colorMat(QString material)
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
