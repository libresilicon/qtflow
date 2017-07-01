#include "buildenvironment.h"
#include "ui_buildenvironment.h"

#include "constants.h"
#include "session.h"

#include <QFile>
#include <QTextStream>
#include <QTableView>


QflowEnvironment::QflowEnvironment(QObject *parent, QString path) :
    QAbstractTableModel(parent),
    settings(new QflowSettings(path)),
    qtflow(new QtFlowSettings),
    vars(settings->table()),
    cwd(path)
{

}

QflowEnvironment::~QflowEnvironment()
{
    delete settings;
    delete qtflow;
}

void QflowEnvironment::save()
{
    map_string_t acc;
    foreach (const auto &e, vars)
        acc.insert(e.first, e.second);
    settings->swap(acc);
    settings->save();
}

int QflowEnvironment::rowCount(const QModelIndex&) const
{
    return vars.count();
}

int QflowEnvironment::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant QflowEnvironment::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() >= 2 ||  index.row() >= vars.size())
        return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    if (index.column() == 0)
        return vars.at(index.row()).first;
    else
        return vars.at(index.row()).second;

    return QVariant();
}

QVariant QflowEnvironment::headerData(int section, Qt::Orientation orientation, int) const
{
    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    if (orientation == Qt::Horizontal)
        return QString("Row %1").arg(section);

    return QVariant();
}

bool QflowEnvironment::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QPair<QString, QString> var;
        if (index.column() == 0)
        {
            var.first = value.toString();
            var.second = vars.at(index.row()).second;
        }
        else
        {
            var.first = vars.at(index.row()).first;
            var.second = value.toString();
        }
        vars.replace(index.row(), var);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags QflowEnvironment::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool QflowEnvironment::insertRows(int position, int rows, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        vars.insert(position, QPair<QString, QString>());
    endInsertRows();
    return true;
}

bool QflowEnvironment::removeRows(int position, int rows, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        vars.removeAt(position);
    endRemoveRows();
    return true;
}


BuildEnvironment::BuildEnvironment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildEnvironment),
    session(Session::Instance()),
    qflow(new QflowEnvironment(this, session.getProject()))
{
    ui->setupUi(this);
    ui->tableView->setModel(qflow);
    ui->tableView->resizeColumnsToContents();
}

BuildEnvironment::~BuildEnvironment()
{
    delete ui;
    delete qflow;
}

void BuildEnvironment::on_buttonBox_rejected()
{

}

void BuildEnvironment::on_buttonBox_accepted()
{
    qflow->save();
}
