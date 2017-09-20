#include "magic3d.h"

Magic3D::Magic3D(QWidget *parent) :
	QDialog(parent),
	project(NULL),
	view(new GLWidget(this)),
	layout(new QVBoxLayout(this))
{
	layout->addWidget(view);
	setLayout(layout);
	setMinimumWidth(500);
	setMinimumHeight(500);
}

void Magic3D::setProject(Project *p)
{
	project = p;
}

void Magic3D::loadFile(QString file)
{
}
