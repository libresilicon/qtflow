#include "schematicseditor.h"
//#include "schematics.yy.h"
#include <QLabel>

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QWidget(parent),
	filePath(QString())
{
}

void SchematicsEditor::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBackground(Qt::white);
	painter.boundingRect(QRect(0,0,400,400),"Test");
	painter.fillRect(QRect(0,0,100,100),Qt::black);
}

SchematicsEditor::~SchematicsEditor()
{
}

void SchematicsEditor::resizeEvent(QResizeEvent*)
{
}

void SchematicsEditor::loadFile(QString file)
{
	filePath = file;
}

void SchematicsEditor::saveFile()
{
}

QString SchematicsEditor::getFilePath()
{
	return filePath;
}

bool SchematicsEditor::changes()
{
	return false;
}
