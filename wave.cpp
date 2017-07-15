#include "wave.h"
#include "ui_wave.h"

#include "vcdparser.h"

#include <QFile>

Wave::Wave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wave)
{
    ui->setupUi(this);
}

Wave::~Wave()
{
    delete ui;
}

void Wave::loadVcd(QString path)
{
    QFile file(path);

    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray content(file.readAll());
    VcdParser parser(content);

}
