#ifndef BUILDSTEPS_H
#define BUILDSTEPS_H

#include "common.h"
#include "project.h"
#include "session.h"

#include <QDialog>
#include <QModelIndex>
#include <QAbstractItemModel>

class BuildScript : public QAbstractListModel
{
    Q_OBJECT

public:
    BuildScript(QObject *parent, QString);
    ~BuildScript();

    void save();

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
    QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex& = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex& = QModelIndex());

private:
    ProjectSettings *settings;
    QtFlowSettings *qtflow;
    QStringList build;
    QString cwd;
};

namespace Ui {
class BuildSteps;
}

class BuildSteps : public QDialog
{
    Q_OBJECT

public:
    explicit BuildSteps(QWidget *parent = 0);
    ~BuildSteps();

public slots:
    void on_contextMenu_requested(const QPoint&);

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::BuildSteps *ui;

    Session &session;

    BuildScript *build;
};

#endif // BUILDSTEPS_H
