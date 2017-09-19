#include "magic3d.h"

Magic3D::Magic3D(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Magic3D),
	project(NULL)
{
	ui->setupUi(this);
}

void Magic3D::setProject(Project *p)
{
	project = p;
}

void Magic3D::loadFile(QString file)
{
	ui->layoutView;
	qDebug() << file;
}
