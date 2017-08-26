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

	foreach(QString key, magicdata->getLayers().keys())
		QTextStream(stdout) << key << '\n';
	/*foreach (const QRect& e, rectangles)
	{
		QColor color = colorMat(layer);
		QPen pen = QPen(color);
		QBrush brush = QBrush(color);
		scene->addRect(e, pen, brush);
	}*/
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

QColor LayoutEditor::colorMat(QString material)
{
	// TODO:
	// make this configuration based!
	// don't hardcode this!

	if (material == "metal1")
		return QColor("lightblue");
	if (material == "metal2")
		return QColor("blue");
	if (material == "metal3")
		return QColor("teal");
	if (material == "metal4")
		return QColor("purple");

	if (material == "m1contact")
		return QColor("yellow");
	if (material == "m2contact")
		return QColor("green");
	if (material == "m3contact")
		return QColor("teal");
	if (material == "m4contact")
		return QColor("teal");

	return QColor("black");
}
