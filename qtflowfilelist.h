#ifndef QTFLOWFILELIST_H
#define QTFLOWFILELIST_H

#include <QWidget>
#include <QFileSystemModel>

#include "project.h"

class QtFlowFileList : public QFileSystemModel
{
	Q_OBJECT

public:
	QtFlowFileList(QObject *parent = Q_NULLPTR, Project *p = Q_NULLPTR);
	void setProject(Project*);

	void refresh();

private:
	QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

	QMap<QString,QString> mapFileModule;

	Project *project;
};

#endif // QTFLOWFILELIST_H
