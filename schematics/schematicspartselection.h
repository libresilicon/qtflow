#ifndef SCHEMATICSPARTSELECTION_H
#define SCHEMATICSPARTSELECTION_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QGraphicsWidget>

#include "ui_partselection.h"

#include "project.h"

#include "qschematics/qschematicspart.h"

class SchematicsPartSelection : public QDialog
{
	Q_OBJECT

public:
	SchematicsPartSelection(QWidget *parent);
	void setProject(Project *p);

public slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	void on_partList_currentItemChanged(QTreeWidgetItem*current, QTreeWidgetItem*previous);

signals:
	void insertPart(QString);

private:
	void updatePartList();

	Ui::PartSelection *ui;
	Project *project;
	QGraphicsScene *partPreview;

	QVector<QTreeWidgetItem*> m_libraries;
	QVector<QTreeWidgetItem*> m_libraryParts;
	QMap<QString,QSchematicsPart*> m_libraryPartWidgets;
};

#endif // SCHEMATICSPARTSELECTION_H
