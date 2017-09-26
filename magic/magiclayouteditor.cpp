#include "magiclayouteditor.h"

MagicLayoutEditor::MagicLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	magicdata(NULL),
	lefdata(NULL),
	project(NULL),
	filePath(QString()),
	editScene(new QGraphicsScene(this))
{
	editScene->setBackgroundBrush(Qt::white);
	
	//sceneRect = QRectF(0,0,this->width(),this->height());
	//editScene->setSceneRect(sceneRect);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
	QGraphicsWireItem *r;
	rects_t layer;
	layer_rects_t rects = magicdata->getRectangles();
	foreach(QString layerN, rects.keys()) {
		layer = rects[layerN];
		foreach (rect_t e, layer) {
			r = new QGraphicsWireItem(e.x1, e.y1, e.x2-e.x1, e.y2-e.y1);
			r->setBrush(QBrush(project->colorMat(layerN)));
			r->setVisible(true);
			editScene->addItem(r);
			wires[layerN].append(r);
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
					if(visibles) if(visibles->layerIsEnabled(layer->getName())) {
						color = project->colorMat(layer->getName());
						foreach(lef::rect_t rect, layer->getRects()) {
							r = new QGraphicsRectItem(rect.x+e.c, rect.y+e.f, rect.w, rect.h, mi);
							r->setBrush(QBrush(color));
							//r->setVisible(true);
							//editScene->addItem(r);
							macro_wires[layer->getName()].append(r);
						}
					}
				}
			}

			foreach (layer, macro->getObstruction()->getLayers()) {
				if(visibles) if(visibles->layerIsEnabled(layer->getName())) {
					color = project->colorMat(layer->getName());
					foreach(lef::rect_t rect, layer->getRects()) {
						r = new QGraphicsRectItem(rect.x+e.c, rect.y+e.f, rect.w, rect.h, mi);
						r->setBrush(QBrush(color));
						//r->setVisible(true);
						//editScene->addItem(r);
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

	addWires();
	addModules();

	editScene->update();
}

void MagicLayoutEditor::redraw()
{
	QGraphicsRectItem *m;
	QGraphicsWireItem *w;
	QGraphicsTextItem *t;
	bool visible;

	visible = true;
	foreach(QString layerN, macro_wires.keys()) {
		visible = (visibles)?(visibles->layerIsEnabled(layerN)):true;
		foreach(m, macro_wires[layerN]) {
			m->setVisible(visible);
		}
	}

	visible = true;
	foreach(QString layerN, wires.keys()) {
		visible = (visibles)?(visibles->layerIsEnabled(layerN)):true;
		foreach(w, wires[layerN]) {
			w->setVisible(visible);
		}
	}

	visible = (visibles)?(visibles->visibleIsEnabled("macro_texts")):true;
	foreach(t, macro_texts) {
		t->setVisible(visible);
	}

	editScene->update();
}

void MagicLayoutEditor::saveFile()
{
	QRectF r;
	wire_layer_t l;
	QGraphicsRectItem *m;
	foreach(QString n, wires.keys()) {
		l = wires[n];
		foreach(m,l) {
			r = m->boundingRect();
		}
	}
}

void MagicLayoutEditor::setProject(Project *p)
{
	project = p;
}

void MagicLayoutEditor::setVisibles(LayoutVisibles *v)
{
	visibles = v;
	if(visibles) connect(visibles, SIGNAL(refreshLayout()), this, SLOT(redraw()));
}

QString MagicLayoutEditor::getFilePath()
{
	return filePath;
}

bool MagicLayoutEditor::changes()
{
	return false;
}
