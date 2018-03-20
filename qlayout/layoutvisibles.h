#ifndef LAYOUTVISIBLES_H
#define LAYOUTVISIBLES_H

#include <QObject>
#include <QDockWidget>
#include <QMenu>

#include "ui_layoutvisibles.h"
#include "tech_reader/techdesignrule.h"
#include "qlayout/tech_data_wrapper.hpp"

class LayoutVisibles : public QDockWidget
{
	Q_OBJECT
public:
	explicit LayoutVisibles(QWidget *parent = nullptr);

	bool typeIsEnabled(QString s);
	void setTechData(TechDataWrapper*toml);

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

	QVector<QTreeWidgetItem*> typeEntries;
	QVector<QTreeWidgetItem*> typeRootEntries;
};

#endif // LAYOUTVISIBLES_H
