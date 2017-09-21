#include "magiclayouteditor.h"

ModuleAreaInfo::ModuleAreaInfo():
	isSelected(false)
{
}

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
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setScene(editScene);
}

void MagicLayoutEditor::mousePressEvent(QMouseEvent * e)
{
	QPointF pt = mapToScene(e->pos());
	foreach(QString key, moduleAreas.keys()) {
		if(moduleAreas[key].area.contains(pt.x(),pt.y())) {
			moduleAreas[key].isSelected = true;
		}
	}
	redraw();
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

void MagicLayoutEditor::drawRectangles()
{
	QColor color;
	rects_t layer;
	layer_rects_t layers = magicdata->getRectangles();
	foreach(QString layerN, layers.keys()) {
		color = project->colorMat(layerN);
		layer = layers[layerN];
		QPen pen = QPen(color);
		QBrush brush = QBrush(color);
		foreach (rect_t e, layer)
		{
			editScene->addRect(QRect(e.x1,e.y1,e.x2-e.x1,e.y2-e.y1), pen, brush);
		}
	}
}

void MagicLayoutEditor::drawModuleInfo()
{
	lef::LEFMacro *macro;
	lef::LEFPin *pin;
	lef::LEFPort *port;
	lef::LEFLayer *layer;

	QColor color;
	QBrush brush;
	QPen pen;
	mods_t mods = magicdata->getModules();
	foreach (module_info e, mods)
	{
		QRect box( e.c, e.f, e.a*(e.x2-e.x1), e.e*(e.y2-e.y1) );

		// fill in library content:
		if(lefdata) if(lefdata->isDefinedMacro(e.module_name)) {
			macro = lefdata->getMacro(e.module_name);
			macro->scaleMacro(box.width(),box.height());

			foreach(pin, macro->getPins()) {
				port = pin->getPort();
				foreach(layer, port->getLayers()) {
					color = project->colorMat(layer->getName());
					pen = QPen(color);
					brush = QBrush(color);
					layer->setOffsetX(e.c);
					layer->setOffsetY(e.f);
					foreach(QRect rect, layer->getRects()) {
						editScene->addRect(rect, pen, brush);
					}
				}
			}

			foreach (layer, macro->getObstruction()->getLayers()) {
				color = project->colorMat(layer->getName());
				pen = QPen(color);
				brush = QBrush(color);
				layer->setOffsetX(e.c);
				layer->setOffsetY(e.f);
				foreach(QRect rect, layer->getRects()) {
					editScene->addRect(rect, pen, brush);
				}
			}

		}

		// write layout details:
		pen = QPen(Qt::black);
		QGraphicsTextItem *instance_name = new QGraphicsTextItem(e.instance_name);
		instance_name->setPos(e.c,e.f);

		moduleAreas[e.instance_name].area = box;
		if(moduleAreas[e.instance_name].isSelected) {
			pen.setColor(Qt::red);
		}
		editScene->addRect(box, pen);
		editScene->addItem(instance_name);
		//editScene->addItem(e.module_name);
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
	redraw();
}

void MagicLayoutEditor::redraw()
{
	drawRectangles();
	drawModuleInfo();
	//fitInView(editScene->sceneRect(), Qt::KeepAspectRatio);
}

void MagicLayoutEditor::saveFile()
{
}

void MagicLayoutEditor::setProject(Project *p)
{
	project = p;
}

QString MagicLayoutEditor::getFilePath()
{
	return filePath;
}

bool MagicLayoutEditor::changes()
{
	return false;
}
