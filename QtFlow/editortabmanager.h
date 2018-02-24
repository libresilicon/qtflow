#ifndef EDITORTABMANAGER_H
#define EDITORTABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>

#include "qeditor/generictexteditorwidget.h"
#include "qeditor/codeeditorwidget.h"
#include "schematics/schematicseditorwidget.h"
#include "magic/magiclayouteditorwidget.h"
#include "def/deflayouteditorwidget.h"
#include "symbols/symboleditorwidget.h"

class EditorTabManager : public QTabWidget
{
	Q_OBJECT
public:
	explicit EditorTabManager(QWidget *parent = nullptr);

signals:
	void fileSaved();

public slots:
	void onContentChanged();
	void onContentSaved();

	void openFile(QString);
	void closeFile(int);

private:
	bool isCode(QString);
	bool isSchematic(QString);
	bool isLayout(QString suffix);
};

#endif // EDITORTABMANAGER_H
