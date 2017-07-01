#include "buildsteps.h"
#include "ui_buildsteps.h"

#include "constants.h"
#include "session.h"
#include "settings.h"

#include <QFile>
#include <QTextStream>

BuildSteps::BuildSteps(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildSteps),
    session(Session::Instance()),
    build(new BuildScript(this, session.getProject()))
{
    ui->setupUi(this);
    ui->listView->setModel(build);
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_contextMenu_requested(const QPoint&)));
}

BuildSteps::~BuildSteps()
{
    delete ui;
    delete build;
}

void BuildSteps::on_contextMenu_requested(const QPoint &)
{

}

void BuildSteps::on_buttonBox_rejected()
{

}

void BuildSteps::on_buttonBox_accepted()
{
    build->save();
}

BuildScript::BuildScript(QObject *parent, QString path) :
    QAbstractListModel(parent),
    settings(new ProjectSettings(path)),
    qtflow(new QtFlowSettings),
    build(settings->value("build").split(",")),
    cwd(path)
{

}

BuildScript::~BuildScript()
{
    delete settings;
    delete qtflow;
}

void BuildScript::save()
{
    settings->set("build", build.join(","));
    settings->save();

    QFile qflow_exec(cwd + QFLOW_EXEC);
    if (qflow_exec.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QString qflowprefix = qtflow->value("qflowprefix");
        QTextStream stream(&qflow_exec);
        stream << TCSH_SHEBANG << endl << endl;
        foreach (const QString &step, build)
        {
            if (step == "synthesize")
                stream << qflowprefix << "/scripts/synthesize.sh " << cwd << " index || exit 1" << endl;
            else if (step == "placement")
                stream << qflowprefix << "/scripts/placement.sh -d " << cwd << " index || exit 1" << endl;
            else if (step == "router")
                stream << qflowprefix << "/scripts/router.sh " << cwd << " index || exit 1" << endl;
            else
                stream << step << endl;
        }
        stream << endl;
        qflow_exec.close();
    }
}

int BuildScript::rowCount(const QModelIndex&) const
{
    return build.count();
}

QVariant BuildScript::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= build.size() || role != Qt::DisplayRole)
        return QVariant();

    return build.at(index.row());
}

QVariant BuildScript::headerData(int section, Qt::Orientation orientation, int) const
{
    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    if (orientation == Qt::Horizontal)
        return QString("Row %1").arg(section);

    return QVariant();
}

bool BuildScript::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        build.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags BuildScript::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool BuildScript::insertRows(int position, int rows, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        build.insert(position, "");
    endInsertRows();
    return true;
}

bool BuildScript::removeRows(int position, int rows, const QModelIndex&)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);
    for (int row = 0; row < rows; ++row)
        build.removeAt(position);
    endRemoveRows();
    return true;
}
