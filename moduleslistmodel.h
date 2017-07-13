#ifndef MODULESLISTMODEL_H
#define MODULESLISTMODEL_H

#include "settings.h"

#include <QAbstractListModel>

class ModulesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ModulesListModel(QObject *parent, ISettings*);
    ~ModulesListModel();

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

    QString file(const QModelIndex& = QModelIndex());

private:
    QList<QString> modules;
    QList<QString> files;
    int topmodule;

    ISettings *qflow;
};

#endif // MODULESLISTMODEL_H
