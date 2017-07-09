#include "modules.h"
#include "ui_modules.h"

#include "constants.h"
#include "settings.h"

#include <QDirIterator>
#include <QDebug>
#include <QListView>
#include <QMessageBox>

Modules::Modules(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modules),
    path(QString()),
    modules(new ModulesModel(parent, new QflowSettings(path)))
{
    ui->setupUi(this);
}

Modules::~Modules()
{
    delete ui;
}

void Modules::refresh()
{
    if (path == QString())
        return;
    refresh(path);
}

void Modules::refresh(QString dir)
{
    path = dir;
    delete modules;
    modules = new ModulesModel(this, new QflowSettings(dir));
    ui->listView->setModel(modules);
}

void Modules::on_setTopModule_clicked()
{
    QString top = modules->data(ui->listView->currentIndex()).toString();
    QString origin = modules->file(ui->listView->currentIndex());
    QFile target(path + "/" + top + ".v");
    QFile file(path + "/" + origin + ".v");
    if (top != origin)
    {
        QMessageBox dialog;
        dialog.setText("Rename " + origin + ".v to " + top + ".v?");
        dialog.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        dialog.setDefaultButton(QMessageBox::No);
        int code = dialog.exec();
        switch (code)
        {
            case QMessageBox::Yes:
                if(!file.rename(target.fileName()))
                    return;
                break;
            default:
                return;
        }
    }

    QflowSettings qflow(path);
    qflow.set(DEFAULT_VERILOG, top);
    qflow.save();
    refresh();
}

void Modules::on_closeButton_clicked()
{
    hide();
}


ModulesModel::ModulesModel(QObject *parent, QflowSettings* settings) :
    QAbstractListModel(parent),
    modules(QList<QString>()),
    files(QList<QString>()),
    topmodule(-1),
    qflow(settings)
{
    QString path  = qflow->value("sourcedir");
    if (path == QString())
        return;

    QRegExp rx("^\\s*module ([a-zA-Z0-9]+)");
    QDirIterator it(path, QStringList() << "*.v", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QFile file(it.next());
        QFileInfo info(file);
        if (info.completeSuffix() == info.suffix() && file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (rx.indexIn(line, 0) != -1)
                {
                    modules << rx.cap(1);
                    files << info.baseName();
                }
            }
            file.close();
        }
    }

    QString index = qflow->value(DEFAULT_VERILOG);
    QList<QString>::iterator is;
    int at;
    for (is = modules.begin(), at = 0; is != modules.end(); ++is, ++at)
        if (*is == index)
            topmodule = at;
}

ModulesModel::~ModulesModel()
{

}

int ModulesModel::rowCount(const QModelIndex &) const
{
    return modules.count();
}

QVariant ModulesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= modules.count())
        return QVariant();

    if (role == Qt::FontRole)
    {
        QFont font;
        if (index.row() != topmodule)
            return font;
        font.setBold(true);
        return font;
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    return modules.at(index.row());
}

QString ModulesModel::file(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= files.count())
        return QString();

    return files.at(index.row());
}
