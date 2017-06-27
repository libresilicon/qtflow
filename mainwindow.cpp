#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "layout.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quit_clicked()
{
    this->close();
}

void MainWindow::on_launch_clicked()
{
    switch (ui->widgets->currentRow()) {
        case 0:
        {
            Layout *l = new Layout(this);
            l->show();
            break;
        }
        default:
            break;
    }
}

void MainWindow::on_widgets_currentRowChanged(int currentRow)
{
    switch (currentRow) {
        case 0:
            ui->description->setText("Layouting widget.");
        break;
        default:
        break;

    }
}
