#ifndef LAYOUTVISIBLES_H
#define LAYOUTVISIBLES_H

#include <QDockWidget>
#include <QMenu>

#include "ui_layoutvisibles.h"

#include "project.h"

class LayoutVisibles : public QDockWidget
{
	Q_OBJECT
public:
	explicit LayoutVisibles(QWidget *parent = nullptr);
	void setProject(Project *p);

	bool typeIsEnabled(QString s);

public slots:
	void changeColor();
	void on_layerList_customContextMenuRequested(const QPoint &pos);
	void handleClick(QTreeWidgetItem *item, int column);
	void handleSearch(QString);

	void onRegisterLayer(QString s);

signals:
	void enabledTypesChanged(QStringList);
	void setCurrentLayer(QString);

private:
	QStringList getEnabledTypes();

	void refreshLists();
	Ui::LayoutVisibles *ui;
	Project *project;

	QVector<QTreeWidgetItem*> typeEntries;
	QVector<QTreeWidgetItem*> typeRootEntries;
};

#endif // LAYOUTVISIBLES_H
