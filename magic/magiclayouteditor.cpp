#include "magiclayouteditor.h"

MagicLayoutEditor::MagicLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(NULL),
	project(NULL),
	activeLayerSelection(NULL),
	visibles(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this)),
	m_scale(1.0)
{
	editScene->setBackgroundBrush(Qt::white);

	setScene(editScene);
	setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

void MagicLayoutEditor::addRectangles()
{
	magic::rects_t layer;
	magic::layer_rects_t rects = magicdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (magic::rect_t e, layer) {
			editScene->addRectangle(layerN, e.x1, e.y1, e.x2-e.x1, e.y2-e.y1);
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
		w = abs(e.x2-e.x1);
		h = abs(e.y2-e.y1);

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
	int x, y, w, h, gridsize;

	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	gridsize = 100;
	x = magicdata->getLowerX();
	y = magicdata->getLowerY();
	w = magicdata->getUpperX()-x;
	h = magicdata->getUpperY()-y;

	if(w<this->width()) w = this->width();
	if(h<this->height()) h = this->height();

	editScene->setSceneRect(x, y, w, h);

	editScene->setDistanceUnit(gridsize); // assumed because magic doesn't provide this info
	addMacroInstances();
	addRectangles();

	editScene->update();
}

void MagicLayoutEditor::saveFileWriteHeader(QTextStream &outputStream)
{
	outputStream << "magic" << endl;
	outputStream << "tech " << project->getTechnology() << endl;
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
						<< r.x()
						<< " "
						<< r.y()
						<< " "
						<< r.x() + r.width()
						<< " "
						<< r.y() + r.height()
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

void MagicLayoutEditor::setProject(Project *p)
{
	project = p;
	editScene->setProject(p);
}

void MagicLayoutEditor::setRecentVisible(QString s)
{
	int index;
	if(!activeLayerSelection) return;

	index = activeLayerSelection->findText(s);
	if ( index != -1 ) { // -1 for not found
		activeLayerSelection->setCurrentIndex(index);
	}

	editScene->setActiveLayer(s);
}

void MagicLayoutEditor::setVisibles(LayoutVisibles *v)
{
	visibles = v;
	if(visibles) {
		connect(visibles, SIGNAL(enabledTypesChanged(QStringList)), editScene, SLOT(onVisibleLayersChanged(QStringList)));
		connect(editScene, SIGNAL(registerLayer(QString)), visibles, SLOT(onRegisterLayer(QString)));
	}
}

void  MagicLayoutEditor::setActiveLayer(QString s)
{
	editScene->setActiveLayer(s);
}

void MagicLayoutEditor::showCellManager()
{
	editScene->showCellManager();
}

void MagicLayoutEditor::zoomIn()
{
	scale(1.1, 1.1);
	editScene->update();
}

void MagicLayoutEditor::showDRC()
{
	editScene->showDRC();
}

void MagicLayoutEditor::zoomOut()
{
	scale(0.9, 0.9);
	editScene->update();
}

QString MagicLayoutEditor::getFilePath()
{
	return filePath;
}

bool MagicLayoutEditor::changes()
{
	return false;
}

void MagicLayoutEditor::setDrawingOperation(drawing_operations o)
{
	editScene->setDrawingOperation(o);
}
