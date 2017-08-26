#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

#include "ieditor.h"
#include "magicdata.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>

class LayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit LayoutEditor(QWidget *parent = nullptr);
	~LayoutEditor();

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

public slots:
	void mousePressEvent(QMouseEvent * e);

protected:
	//void resizeEvent(QResizeEvent*);
	//void paintEvent(QPaintEvent *event);
	QColor colorMat(QString material);
	void drawBoxes();

private:
	QString filePath;
	QGraphicsScene *editScene;
	magic::MagicData *magicdata;

};

#endif // LAYOUTEDITOR_H
