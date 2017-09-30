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

signals:
	void refreshLayout();

public slots:
	void changeColor();
	void on_layerList_customContextMenuRequested(const QPoint &pos);
	void handleClick(const QModelIndex &index);
	void handleSearch(QString);

private:
	void refreshLists();
	Ui::LayoutVisibles *ui;
	Project *project;

	QVector<QTreeWidgetItem*> typeEntries;
};

#endif // LAYOUTVISIBLES_H
