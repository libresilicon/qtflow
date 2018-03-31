#ifndef GENERICLAYOUTEDITORWIDGET_H
#define GENERICLAYOUTEDITORWIDGET_H

#include <QDockWidget>
#include <QMainWindow>

#include "genericlayouteditor.h"
#include "qeditor/editorwidget.h"

#include "qlayout/layoutvisibles.h"

#include <iostream>

class GenericLayoutEditorWidget : public EditorWidget
{
	Q_OBJECT
public:
	GenericLayoutEditorWidget(QWidget *parent = nullptr);
	void loadFile(QString);
	void loadLEF(QString);
	void loadGDS(QString);
	void setTechnologyData(TechDataWrapper* toml);
	void setLambdaUnit(QString s);
	void setLambdaValue(qreal i);
	QString getFilePath();

protected:
	QAction *addToolbarAction(QPixmap pm, QString str);
	void setUpCentralWidget(GenericLayoutEditor *area);

public slots:
	void onContentChanged();
	void saveFile();
	void drawingOperation();

private:
	void addDrawingLayerSelection();

	QMap<drawing_operations,QAction*> m_drawingOperations;
	LayoutVisibles *m_layoutVisibles;
	QComboBox *m_activeLayer;
};

#endif // GENERICLAYOUTEDITORWIDGET_H
