#include "magiclayouteditor.h"
#include <QDateTime>

MagicLayoutEditor::MagicLayoutEditor(QWidget *parent) :
	GenericLayoutEditor(parent),
	magicdata(NULL)
{
}

void MagicLayoutEditor::addRectangles()
{
	magic::rects_t layer;
	magic::layer_rects_t rects = magicdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (magic::rect_t e, layer) {
			editScene->addRectangle(layerN, qFabs(e.x1), -qFabs(e.y1), qFabs(e.x2-e.x1), -qFabs(e.y2-e.y1));
		}
	}
}

void MagicLayoutEditor::addMacroInstances()
{
	QString orient;
	qreal x,y,w,h;
	magic::mods_t mods;
	mods = magicdata->getModules();
	foreach (magic::module_info e, mods) {
		// adding boxes for macros
		x = e.c;
		y = e.f;
		w = qFabs(e.x2-e.x1);
		h = qFabs(e.y2-e.y1);

		if((e.a==0)&&(e.b==-1)&&(e.d==1)&&(e.e==0)) { // West
			orient = "W";
		} else if((e.a==0)&&(e.b==1)&&(e.d==-1)&&(e.e==0)) { // East
			orient = "E";
		} else if((e.a==-1)&&(e.b==0)&&(e.d==0)&&(e.e==-1)) { // South
			orient = "S";
		} else if((e.a==1)&&(e.b==0)&&(e.d==0)&&(e.e==-1)) { // South Flipped
			orient = "FS";
		} else if((e.a==1)&&(e.b==0)&&(e.d==0)&&(e.e==1)) { // North
			orient = "N";
		} else if((e.a==-1)&&(e.b==0)&&(e.d==0)&&(e.e==1)) { // North Flippled
			orient = "FN";
		}

		editScene->addMacro(e.module_name, e.instance_name, x, y, w, h, orient);
	}
}

void MagicLayoutEditor::loadFile(QString file)
{
	int x, y, w, h;

	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	x = qFabs(magicdata->getLowerX());
	y = qFabs(magicdata->getLowerY());
	w = qFabs(magicdata->getUpperX()-x);
	h = qFabs(magicdata->getUpperY()-y);

	//if(w<this->width()) w = this->width();
	//if(h<this->height()) h = this->height();

	editScene->setSceneRect(0, 0, w*LAYOUT_SCALE_FACTOR, -h*LAYOUT_SCALE_FACTOR);

	addMacroInstances();
	addRectangles();

	editScene->update();
}

void MagicLayoutEditor::saveFileWriteHeader(QTextStream &outputStream)
{
	outputStream << "magic" << endl;
	outputStream << "magscale 1 2" << endl;
	outputStream << "timestamp " << QDateTime::currentMSecsSinceEpoch() << endl;
}

void MagicLayoutEditor::saveFileWriteRects(QTextStream &outputStream)
{
	foreach(QString n, editScene->getLayers()) {
		outputStream << "<< " << n << " >>" << endl;
		foreach(QLayoutRectItem *m, editScene->getRectangles(n)) {
			foreach(QRectF r, m->getStripes()) {
				outputStream
						<< "rect "
						<< (int)(r.x()/LAYOUT_SCALE_FACTOR)
						<< " "
						<< -(int)(r.y()/LAYOUT_SCALE_FACTOR)
						<< " "
						<< (int)((r.x()+r.width())/LAYOUT_SCALE_FACTOR)
						<< " "
						<< -(int)((r.y()+r.height())/LAYOUT_SCALE_FACTOR)
						<< endl;
			}
		}
	}
}

void MagicLayoutEditor::saveFileWriteMacros(QTextStream &outputStream)
{
	//foreach(QGraphicsMacroItem *m, macros) {
		//r = m->rect();
	//	m->getMacroName();
	//}
}

void MagicLayoutEditor::saveFile()
{
	qDebug() << "Saving " << filePath;

	QFile magicFile(filePath);
	if(magicFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&magicFile);
		saveFileWriteHeader(outputStream);
		saveFileWriteRects(outputStream);
		saveFileWriteMacros(outputStream);
		outputStream << "<< end >>" << endl;
		magicFile.close();
	}
}

