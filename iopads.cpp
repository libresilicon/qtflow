#include "iopads.h"
#include "ui_iopads.h"

IOPads::IOPads(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IOPads)
{
    ui->setupUi(this);
}

IOPads::~IOPads()
{
    delete ui;
}
