#include "magic3d.h"

Magic3D::Magic3D(QWidget *parent) :
	QDialog(parent),
	project(NULL),
	layout(new QVBoxLayout(this)),
	view(new GLWidget(this)),
	toolbar(new QToolBar(this))
{
	QAction *button;
	QPixmap pixmap;

	pixmap = QPixmap(":/axis3d_front.svg");
	button = new QAction(pixmap,"Front", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_front()));

	pixmap = QPixmap(":/axis3d_back.svg");
	button = new QAction(pixmap,"Back", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_back()));

	pixmap = QPixmap(":/axis3d_top.svg");
	button = new QAction(pixmap,"Top", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_top()));

	pixmap = QPixmap(":/axis3d_bottom.svg");
	button = new QAction(pixmap,"Bottom", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_bottom()));

	pixmap = QPixmap(":/axis3d_left.svg");
	button = new QAction(pixmap,"Left", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_left()));

	pixmap = QPixmap(":/axis3d_right.svg");
	button = new QAction(pixmap,"Right", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_right()));

	pixmap = QPixmap(":/axis3d.svg");
	button = new QAction(pixmap,"Side", toolbar);
	toolbar->addAction(button);
	connect(button, SIGNAL(triggered(bool)), view, SLOT(axis3d_side()));

	layout->addWidget(toolbar);
	layout->addWidget(view);
	setLayout(layout);
	setMinimumWidth(500);
	setMinimumHeight(500);
}

void Magic3D::setProject(Project *p)
{
	project = p;
	view->setProject(project);
}

void Magic3D::loadFile(QString file)
{
	view->loadFile(file);
}
