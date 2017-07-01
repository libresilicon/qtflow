#include "mainedit.h"
#include "ui_mainedit.h"

#include <QTreeView>

MainEdit::MainEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainEdit),
    filesystem(new QFileSystemModel)
{
    ui->setupUi(this);
    ui->filesView->setModel(filesystem);
    loadProject(QDir::currentPath());
}

MainEdit::~MainEdit()
{
    delete ui;
    delete filesystem;
}

void MainEdit::loadProject(QString path)
{
    filesystem->setRootPath(path);
    ui->filesView->setRootIndex(filesystem->index(path));
}
