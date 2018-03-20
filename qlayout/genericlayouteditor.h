#ifndef GENRICLAYOUTEDITOR_H
#define GENRICLAYOUTEDITOR_H

#include <QGridLayout>
#include <QAbstractScrollArea>
#include <QFileInfo>
#include <QResource>
#include <QDebug>
#include <QTemporaryDir>
#include <QAction>
#include <QComboBox>
#include <QGraphicsView>

#include "qlayoutscene.h"
#include "layoutvisibles.h"

class GenericLayoutEditor : public QGraphicsView
{
	Q_OBJECT
public:
	explicit GenericLayoutEditor(QWidget *parent = nullptr);

	virtual void loadFile(QString) {}
	void loadGDS(QString);
	void loadLEF(QString s);
	virtual void saveFile() {}
    
	QString getFilePath();
    
	void setDrawingOperation(drawing_operations o);
    
	bool changes();

signals:
	void contentSaved();
	void registerLayer(QString);

public slots:
	void onRegisterLayer(QString s);
	void setActiveLayer(QString);

	void zoomIn();
	void zoomOut();
	void showDRC();
	void showCellManager();

private:
	QRectF sceneRect;
	LayoutVisibles *visibles;
	qreal m_scale;

protected:
	QLayoutScene *editScene;
	QString filePath;
};

#endif // GENRICLAYOUTEDITOR_H
