#ifndef DEFLAYOUTEDITOR_H
#define DEFLAYOUTEDITOR_H

#include "ieditor.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QGridLayout>

namespace def {
	class DEFData;
}

namespace lef {
	class LEFData;
}

class DEFLayoutEditor : public QGraphicsView, public IEditor
{
	Q_OBJECT

public:
	explicit DEFLayoutEditor(QWidget *parent = nullptr);

	void loadFile(QString);
	void saveFile();
	QString getFilePath();

	bool changes();

public slots:
	void mousePressEvent(QMouseEvent * e);

protected:
	QColor colorMat(QString material);
	void drawRectangles();
	void drawModuleInfo();

private:
	QString filePath;
	QGraphicsScene *editScene;
	def::DEFData *defdata;
	lef::LEFData *lefdata;
};

#endif // DEFLAYOUTEDITOR_H
