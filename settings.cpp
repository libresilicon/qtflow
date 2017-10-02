#include "settings.h"

#include <QDebug>
#include <QListWidget>
#include <QStackedWidget>
#include <QTextStream>
#include <QFileDialog>

#include <KTextEditor/ConfigPage>
#include <KTextEditor/Editor>

Settings::Settings(QWidget *parent, QSettings *s) :
    QDialog(parent),
	ui(new Ui::Settings),
    settings(s),
    textEditor(KTextEditor::Editor::instance())
{
	ui->setupUi(this);
	connect(ui->yosysButton,SIGNAL(clicked(bool)),this,SLOT(selectYosys_triggered()));
	connect(ui->graywolfButton,SIGNAL(clicked(bool)),this,SLOT(selectGraywolf_triggered()));
	connect(ui->qrouterButton,SIGNAL(clicked(bool)),this,SLOT(selectQRouter_triggered()));
	connect(ui->icarusButton,SIGNAL(clicked(bool)),this,SLOT(selectIcarus_triggered()));

    // pages
    page1 = ui->stackedWidget->currentWidget();
    page2 = textEditor->configPage(0, ui->page2);
    page3 = textEditor->configPage(1, ui->page3);
    page4 = textEditor->configPage(2, ui->page4);
    page5 = textEditor->configPage(3, ui->page5);

    // page1
    ui->pathYosys->setText(settings->value("yosys").toString());
    ui->pathGraywolf->setText(settings->value("graywolf").toString());
    ui->pathQRouter->setText(settings->value("qrouter").toString());
    ui->pathIcarus->setText(settings->value("icarus").toString());
}

void Settings::selectYosys_triggered()
{
	ui->pathYosys->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectGraywolf_triggered()
{
	ui->pathGraywolf->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectQRouter_triggered()
{
	ui->pathQRouter->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::selectIcarus_triggered()
{
	ui->pathIcarus->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath()));
}

void Settings::applyEnvironment()
{
	settings->setValue("yosys",ui->pathYosys->displayText());
	settings->setValue("graywolf",ui->pathGraywolf->displayText());
	settings->setValue("qrouter",ui->pathQRouter->displayText());
	settings->setValue("icarus",ui->pathIcarus->displayText());
}

void Settings::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void Settings::on_buttonBox_accepted()
{
	applyEnvironment();
	page2->apply();
	page3->apply();
	page4->apply();
	page5->apply();

	emit(syncSettings());
	emit(on_buttonBox_rejected());
}

void Settings::on_buttonBox_rejected()
{
    close();
}
