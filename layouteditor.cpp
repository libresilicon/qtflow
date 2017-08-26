#include "layouteditor.h"

LayoutEditor::LayoutEditor(QWidget *parent) :
	QWidget(parent),
	filePath(QString()),
	magicdata(new magic::MagicData())
{
}

void LayoutEditor::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBackground(Qt::white);
	painter.boundingRect(QRect(0,0,400,400),"Test");
	painter.fillRect(QRect(0,0,100,100),Qt::black);
}

LayoutEditor::~LayoutEditor()
{
}

void LayoutEditor::resizeEvent(QResizeEvent*)
{
}

void LayoutEditor::loadFile(QString file)
{
	filePath = file;
	magicdata->loadFile(file);
}

void LayoutEditor::saveFile()
{
}

QString LayoutEditor::getFilePath()
{
	return filePath;
}

bool LayoutEditor::changes()
{
	return false;
}
