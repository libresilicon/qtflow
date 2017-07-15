#include "wave.h"
#include "ui_wave.h"

#include "vcdparser.h"

#include <QFile>
#include <QTreeView>

Wave::Wave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wave),
    tree(new VcdTreeModel)
{
    ui->setupUi(this);
    ui->treeView->setModel(tree);
}

Wave::~Wave()
{
    delete ui;
    delete tree;
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

    tree->setVcd(parser.getVcd());
}
