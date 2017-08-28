#ifndef EDITORTABMANAGER_H
#define EDITORTABMANAGER_H

#include <QWidget>
#include <QTabWidget>
#include "codeeditorwidget.h"
#include "schematicseditorwidget.h"
#include "layouteditorwidget.h"

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
