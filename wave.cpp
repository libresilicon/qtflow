#include "wave.h"
#include "ui_wave.h"

#include "vcdparser.h"

#include <QFile>
#include <QTreeView>
#include <QListView>
#include <QGraphicsView>
#include <QMimeData>

Wave::Wave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Wave),
    tree(new VcdTreeModel),
    list(new VcdListModel),
    scene(new QGraphicsScene)
{
    ui->setupUi(this);
    ui->treeView->setModel(tree);
    ui->listView->setModel(list);
    ui->graphicsView->setScene(scene);

    ui->treeView->setDragEnabled(true);
    ui->treeView->setDragDropMode(QAbstractItemView::DragOnly);
    ui->treeView->setDefaultDropAction(Qt::CopyAction);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDragDropMode(QAbstractItemView::DropOnly);
    ui->listView->setDefaultDropAction(Qt::CopyAction);
}

Wave::~Wave()
{
    delete ui;
    delete tree;
    delete list;
    delete scene;
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
