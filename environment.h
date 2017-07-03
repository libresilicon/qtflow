#ifndef BUILDENVIRONMENT_H
#define BUILDENVIRONMENT_H

#include "session.h"
#include "settings.h"

#include <QDialog>
#include <QAbstractItemModel>

class QflowEnvironment : public QAbstractTableModel
{
    Q_OBJECT

public:
    QflowEnvironment(QObject *parent, QString);
    ~QflowEnvironment();

    void save();

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex& = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex& = QModelIndex());

private:
    QflowSettings *settings;
    QtFlowSettings *qtflow;
    table_string_t vars;
    QString cwd;
};

namespace Ui {
class Environment;
}

class Environment : public QDialog
{
    Q_OBJECT

public:
    explicit Environment(QWidget *parent = 0);
    ~Environment();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::Environment *ui;

    Session &session;

    QflowEnvironment *qflow;
};

#endif // BUILDENVIRONMENT_H
