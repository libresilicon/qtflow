#ifndef EDITORTABMANAGER_H
#define EDITORTABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>

#include "generictexteditorwidget.h"
#include "codeeditorwidget.h"
#include "schematics/schematicseditorwidget.h"
#include "magic/magiclayouteditorwidget.h"
#include "def/deflayouteditorwidget.h"
#include "project.h"
#include "layoutvisibles.h"

class EditorTabManager : public QTabWidget
{
	Q_OBJECT
public:
	explicit EditorTabManager(QWidget *parent = nullptr);

	void setProject(Project *p);
	void setVisibles(LayoutVisibles *v);

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

	Project *project;
	LayoutVisibles *visibles;
};

#endif // EDITORTABMANAGER_H
