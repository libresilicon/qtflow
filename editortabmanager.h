#ifndef EDITORTABMANAGER_H
#define EDITORTABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include "codeeditorwidget.h"
#include "schematicseditorwidget.h"

class EditorTabManager : public QWidget
{
	Q_OBJECT
public:
	explicit EditorTabManager(QWidget *parent = nullptr);
	bool isCode(QString);
	bool isSchematic(QString);

signals:

public slots:
	void onContentChanged();
	void onContentSaved();

	void openFile(QString);
	void closeFile(int);

private:
	QTabWidget *editArea;
};

#endif // EDITORTABMANAGER_H
