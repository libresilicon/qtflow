#include "schematicseditor.h"
#include <QLabel>

SchematicsEditor::SchematicsEditor(QWidget *parent):
	QGraphicsView(parent),
	editScene(new QGraphicsScene(this)),
	filePath(QString())
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
