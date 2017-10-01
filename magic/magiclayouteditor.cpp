#include "magiclayouteditor.h"

MagicLayoutEditor::MagicLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(NULL),
	lefdata(NULL),
	project(NULL),
	activeLayerSelection(NULL),
	visibles(NULL),
	filePath(QString()),
	editScene(new QGraphicsScene(this)),
	recentOperation(DRAWING_OPERATION_NONE),
	recentRectangle(NULL)
{
	editScene->setBackgroundBrush(Qt::white);
	setScene(editScene);
}

void MagicLayoutEditor::resizeEvent(QResizeEvent *event)
{
	redraw();
}

void MagicLayoutEditor::scrollContentsBy(int dx, int dy)
{
	//sceneRect = QRectF(sceneRect.x()+dx,sceneRect.y()+dy,this->width(),this->height());
	//editScene->setSceneRect(sceneRect);
	redraw();
}

void MagicLayoutEditor::addWires()
{
	QGraphicsLayoutRectItem *r;
	rects_t layer;
	layer_rects_t rects = magicdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (rect_t e, layer) {
			r = new QGraphicsLayoutRectItem(e.x1, e.y1, e.x2-e.x1, e.y2-e.y1);
			r->setBrush(QBrush(project->colorMat(layerN)));
			r->setVisible(true);
			editScene->addItem(r);
			layer_rects[layerN].append(r);
		}
	}
}

void MagicLayoutEditor::addModules()
{
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	lef::LEFPort *port;
	lef::LEFLayer *layer;

	QGraphicsRectItem *r;
	QGraphicsMacroItem *mi;
	QColor color;
	mods_t mods;

	mods = magicdata->getModules();
	foreach (module_info e, mods) {
		// adding boxes for macros
		mi = new QGraphicsMacroItem(e.x1+e.c, e.y1+e.f, e.a*(e.x2-e.x1), e.e*(e.y2-e.y1));
		mi->setVisible(true);

		// fill in library content:
		if(lefdata) if(lefdata->isDefinedMacro(e.module_name)) {
			mi->setMacroName(e.instance_name);
			editScene->addItem(mi);

			macro = lefdata->getMacro(e.module_name);
			macro->scaleMacro(e.a*(e.x2-e.x1), e.e*(e.y2-e.y1));

			foreach(pin, macro->getPins()) {
				port = pin->getPort();
				foreach(layer, port->getLayers()) {
					if(visibles) if(visibles->typeIsEnabled(layer->getName())) {
						color = project->colorMat(layer->getName());
						foreach(lef::rect_t rect, layer->getRects()) {
							r = new QGraphicsRectItem(rect.x+e.c, rect.y+e.f, rect.w, rect.h, mi);
							r->setBrush(QBrush(color));
							macro_wires[layer->getName()].append(r);
						}
					}
				}
			}

			foreach (layer, macro->getObstruction()->getLayers()) {
				if(visibles) if(visibles->typeIsEnabled(layer->getName())) {
					color = project->colorMat(layer->getName());
					foreach(lef::rect_t rect, layer->getRects()) {
						r = new QGraphicsRectItem(rect.x+e.c, rect.y+e.f, rect.w, rect.h, mi);
						r->setBrush(QBrush(color));
						macro_wires[layer->getName()].append(r);
					}
				}
			}

		}

		// write layout details:
		QGraphicsTextItem *instance_name = new QGraphicsTextItem(e.instance_name, mi);
		instance_name->setPos(e.c,e.f);
		instance_name->setVisible(true);
		macro_texts.append(instance_name);
		macros.append(mi);
	}
}

void MagicLayoutEditor::loadFile(QString file)
{
	QString filedest;
	QTemporaryDir temporaryDir;
	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	if(project->getTechnology()==magicdata->getTechnology()) {
		if(lefdata) delete lefdata;
		lefdata = new lef::LEFData();
		foreach(QString filename, project->getProcessFiles()) {
			filedest = temporaryDir.path()+"/cells.lef";
			QFile::copy(filename, filedest);
			if(QFile(filedest).exists()) {
				lefdata->loadFile(filedest);
			}
		}
	}

	addModules();
	addWires();

	editScene->update();
}

void MagicLayoutEditor::redraw()
{
	QGraphicsRectItem *m;
	QGraphicsLayoutRectItem *w;
	QGraphicsTextItem *t;
	bool visible;

	visible = true;
	foreach(QString layerN, macro_wires.keys()) {
		visible = (visibles)?(visibles->typeIsEnabled(layerN)):true;
		foreach(m, macro_wires[layerN]) {
			m->setVisible(visible);
		}
	}

	visible = true;
	foreach(QString layerN, layer_rects.keys()) {
		visible = (visibles)?(visibles->typeIsEnabled(layerN)):true;
		foreach(w, layer_rects[layerN]) {
			w->setVisible(visible);
		}
	}

	visible = (visibles)?(visibles->typeIsEnabled("comment")):true;
	foreach(t, macro_texts) {
		t->setVisible(visible);
	}

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
	rects_layer_t l;
	QRectF r;
	QGraphicsRectItem *m;
	foreach(QString n, layer_rects.keys()) {
		outputStream << "<< " << n << " >>" << endl;
		l = layer_rects[n];
		foreach(m,l) {
			r = m->rect();
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

void MagicLayoutEditor::saveFileWriteMacros(QTextStream &outputStream)
{
	foreach(QGraphicsMacroItem *m, macros) {
		//r = m->rect();
		m->getMacroName();
	}
}

void MagicLayoutEditor::saveFile()
{
	qDebug() << "Saving " << filePath;

	QFile magicFile(filePath);
	if(magicFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&magicFile);
		saveFileWriteHeader(outputStream);
		saveFileWriteRects(outputStream);
		//saveFileWriteMacros(&outputStream);
		outputStream << "<< end >>" << endl;
		magicFile.close();
	}
}

void MagicLayoutEditor::setDrawingOperation(drawing_operations o)
{
	recentOperation = o;
}

void MagicLayoutEditor::setProject(Project *p)
{
	project = p;
}

void MagicLayoutEditor::visibles_action(QString s)
{
	redraw();
	setRecentVisible(s);
}

void MagicLayoutEditor::setRecentVisible(QString s)
{
	int index;
	if(!activeLayerSelection) return;

	qDebug() << s;

	index = activeLayerSelection->findText(s);
	if ( index != -1 ) { // -1 for not found
		activeLayerSelection->setCurrentIndex(index);
	}
}

void MagicLayoutEditor::setVisibles(LayoutVisibles *v)
{
	visibles = v;
	if(visibles) {
		connect(visibles, SIGNAL(refreshLayout(QString)), this, SLOT(visibles_action(QString)));
	}
}

void MagicLayoutEditor::setActiveLayerSelection(QComboBox *s)
{
	activeLayerSelection = s;
}

QString MagicLayoutEditor::getFilePath()
{
	return filePath;
}

bool MagicLayoutEditor::changes()
{
	return false;
}

void MagicLayoutEditor::mousePressEvent(QMouseEvent *event)
{
	lastOrig = mapToScene(event->pos());
	QString material = "metal1";
	switch(recentOperation) {
		case DRAWING_OPERATION_RECTANGLE:
			if(activeLayerSelection) material = activeLayerSelection->currentText();
			recentRectangle = new QGraphicsLayoutRectItem(lastOrig.x(),lastOrig.y(),1,1);
			recentRectangle->setVisible(true);
			recentRectangle->setBrush(QBrush(project->colorMat(material)));
			editScene->addItem(recentRectangle);
			layer_rects[material].append(recentRectangle);
			emit(contentChanged());
			break;
	}
	editScene->update();
}

void MagicLayoutEditor::mouseMoveEvent(QMouseEvent *event)
{
	qreal dx, dy;
	QPointF pt;

	pt = mapToScene(event->pos());
	switch(recentOperation) {
		case DRAWING_OPERATION_RECTANGLE:
			if(recentRectangle) {
				dx=pt.x()-lastOrig.x();
				dy=pt.y()-lastOrig.y();
				recentRectangle->setRect(lastOrig.x(),lastOrig.y(),dx,dy);;
			}
			break;
	}
	editScene->update();
}

void MagicLayoutEditor::mouseReleaseEvent(QMouseEvent *event)
{
	QPointF pt;
	pt = mapToScene(event->pos());
	if(recentRectangle) {
		recentRectangle = NULL;
	}
	editScene->update();
}
