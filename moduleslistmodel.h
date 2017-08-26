#ifndef MODULESLISTMODEL_H
#define MODULESLISTMODEL_H

#include <QSettings>
#include <QAbstractListModel>

class ModulesListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	ModulesListModel(QObject *parent, QString sourcedir);
	~ModulesListModel();

	int rowCount(const QModelIndex& = QModelIndex()) const override;
	QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

	QString file(const QModelIndex& = QModelIndex());

	void setTestBench(QString);
	void setTopLevel(QString);

private:
	QList<QString> modules;
	QList<QString> files;
	QString sourcedir;
	QString toplevel;
	QString testbench;
};

#endif // MODULESLISTMODEL_H
