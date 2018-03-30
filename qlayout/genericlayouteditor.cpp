#include "genericlayouteditor.h"

GenericLayoutEditor::GenericLayoutEditor(QWidget *parent) :
	QGraphicsView(parent),
	visibles(NULL),
	filePath(QString()),
	editScene(new QLayoutScene(this)),
	m_scale(1.0)
{
	QGLWidget *glw;
	editScene->setBackgroundBrush(Qt::white);
	setScene(editScene);
	glw = new QGLWidget(QGLFormat(QGL::SampleBuffers |QGL::DirectRendering));
	setViewport(glw);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setInteractive(true);
	setAlignment( Qt::AlignBottom | Qt::AlignLeft );
	//setDragMode(QGraphicsView::ScrollHandDrag);
	connect(editScene, SIGNAL(registerLayer(QString)), this, SLOT(onRegisterLayer(QString)));
	connect(parent, SIGNAL(enabledTypesChanged(QStringList)), editScene, SLOT(onVisibleLayersChanged(QStringList)));
	connect(parent, SIGNAL(setCurrentLayer(QString)), editScene, SLOT(setActiveLayer(QString)));
}

void GenericLayoutEditor::loadGDS(QString f)
{
	editScene->setGDS(f);
}

void GenericLayoutEditor::setTechData(TechDataWrapper* toml)
{
	editScene->setTechData(toml);
}

void GenericLayoutEditor::onRegisterLayer(QString s)
{
	emit registerLayer(s);
}

void GenericLayoutEditor::loadLEF(QString s)
{
	editScene->setLEF(s);
}

void GenericLayoutEditor::zoomIn()
{
	scale(1.1, 1.1);
	editScene->update();
}

void GenericLayoutEditor::zoomOut()
{
	scale(0.9, 0.9);
	editScene->update();
}

void GenericLayoutEditor::showDRC()
{
	editScene->showDRC();
}

void GenericLayoutEditor::onVisibleLayersChanged(QStringList l)
{
	editScene->onVisibleLayersChanged(l);	
}

void GenericLayoutEditor::setActiveLayer(QString s)
{
	editScene->setActiveLayer(s);
}

void GenericLayoutEditor::showCellManager()
{
	editScene->showCellManager();
}

QString GenericLayoutEditor::getFilePath()
{
	return filePath;
}

bool GenericLayoutEditor::changes()
{
	return false;
}

void GenericLayoutEditor::setDrawingOperation(drawing_operations o)
{
	editScene->setDrawingOperation(o);
}
