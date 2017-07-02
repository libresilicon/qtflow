#include "savechanges.h"
#include "ui_savechanges.h"

#include <QLabel>
#include <QFileInfo>

SaveChanges::SaveChanges(QWidget *parent, int i, QString path) :
    QDialog(parent),
    ui(new Ui::SaveChanges),
    index(i)
{
    ui->setupUi(this);

    QFileInfo info(path);
    if (path != "")
        ui->label->setText(QString("Save changes to %1?").arg(info.fileName()));
    else
        ui->label->setText(QString("Save changes?"));
}

SaveChanges::~SaveChanges()
{
    delete ui;
}

void SaveChanges::on_buttonBox_rejected()
{
    emit rejected();
}

void SaveChanges::on_buttonBox_accepted()
{
    emit accepted(index);
}
