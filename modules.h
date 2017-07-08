#ifndef MODULES_H
#define MODULES_H

#include "settings.h"

#include <QDialog>
#include <QAbstractListModel>

class ModulesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ModulesModel(QObject *parent, QString = QString());
    ~ModulesModel();

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

    QString file(const QModelIndex& = QModelIndex());

private:
    QList<QString> modules;
    QList<QString> files;
    int topmodule;

    QflowSettings *qflow;
};

namespace Ui {
class Modules;
}

class Modules : public QDialog
{
    Q_OBJECT

public:
    explicit Modules(QWidget *parent = 0);
    ~Modules();

    void refresh();
    void refresh(QString);

private slots:
    void on_setTopModule_clicked();

    void on_closeButton_clicked();

private:
    Ui::Modules *ui;

    QString path;
    ModulesModel *modules;
};

#endif // MODULES_H
