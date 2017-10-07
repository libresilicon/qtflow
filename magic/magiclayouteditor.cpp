#include "magiclayouteditor.h"

MagicLayoutEditor::MagicLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(NULL),
	lefdata(NULL),
	project(NULL),
	activeLayerSelection(NULL),
	visibles(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this)),
	m_scale(1.0)
{
	editScene->setBackgroundBrush(Qt::white);

	setScene(editScene);
	//setRenderHint(QPainter::Antialiasing);
	//setCacheMode(QGraphicsView::CacheBackground);
	//setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void MagicLayoutEditor::addRectangles()
{
	rects_t layer;
	layer_rects_t rects = magicdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (rect_t e, layer) {
			editScene->addRectangle(layerN, e.x1, e.y1, e.x2-e.x1, e.y2-e.y1);
		}
	}
}

void MagicLayoutEditor::addMacroInstances()
{
	mods_t mods;
	mods = magicdata->getModules();
	foreach (module_info e, mods) {
		// adding boxes for macros
		editScene->addMacro(e.module_name, e.instance_name, e.x1+e.c, e.y1+e.f, e.a*(e.x2-e.x1), e.e*(e.y2-e.y1));
	}
}

void MagicLayoutEditor::loadFile(QString file)
{
	int x, y, w, h;
	QString filedest;
	QTemporaryDir temporaryDir;
	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	x = magicdata->getLowerX();
	y = magicdata->getLowerY();
	w = magicdata->getUpperX()-x;
	h = magicdata->getUpperY()-y;

	if(w<this->width()) w = this->width();
	if(h<this->height()) h = this->height();

	if(project->getTechnology()==magicdata->getTechnology()) {
		if(lefdata) delete lefdata;
		lefdata = new lef::LEFData();
		foreach(QString filename, project->getLibraryFiles()) {
			filedest = temporaryDir.path()+"/cells.lef";
			QFile::copy(filename, filedest);
			if(QFile(filedest).exists()) {
				lefdata->loadFile(filedest);
			}
		}
		editScene->setLEF(lefdata);
	}

	editScene->setGridSize(10);
	editScene->setSceneRect(x,y,w,h);

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

void MagicLayoutEditor::visibles_action(QString s)
{
	QStringList l;
	setRecentVisible(s);
	editScene->setVisibleLayers(visibles->getEnabledTypes());
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
		connect(visibles, SIGNAL(refreshLayout(QString)), this, SLOT(visibles_action(QString)));
	}
}

void  MagicLayoutEditor::setActiveLayer(QString s)
{
	editScene->setActiveLayer(s);
}

void MagicLayoutEditor::zoomIn()
{
	scale(1.1, 1.1);
	editScene->update();
}

void MagicLayoutEditor::zoomOut()
{
	scale(0.9, 0.9);
	editScene->update();
}

void MagicLayoutEditor::setActiveLayerSelection(QComboBox *s)
{
	activeLayerSelection = s;
	if(activeLayerSelection) {
		editScene->setActiveLayer(activeLayerSelection->currentText());
		connect(activeLayerSelection,SIGNAL(currentTextChanged(QString)),this,SLOT(setActiveLayer(QString)));
	}
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

