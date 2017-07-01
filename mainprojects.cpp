#include "mainprojects.h"
#include "ui_mainprojects.h"

MainProjects::MainProjects(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainProjects)
{
    ui->setupUi(this);
}

MainProjects::~MainProjects()
{
    delete ui;
}
