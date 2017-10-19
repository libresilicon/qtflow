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

    // pages
    page1 = ui->stackedWidget->currentWidget();

    page2 = textEditor->configPage(0, ui->page2);
	ui->page2->layout()->addWidget(page2);

    page3 = textEditor->configPage(1, ui->page3);
	ui->page3->layout()->addWidget(page3);

    page4 = textEditor->configPage(2, ui->page4);
	ui->page4->layout()->addWidget(page4);

    page5 = textEditor->configPage(3, ui->page5);
	ui->page5->layout()->addWidget(page5);

    // page1
    ui->pathYosys->setText(settings->value("yosys").toString());
	ui->pathYosysABC->setText(settings->value("yosys-abc").toString());
    ui->pathGraywolf->setText(settings->value("graywolf").toString());
    ui->pathQRouter->setText(settings->value("qrouter").toString());
    ui->pathIcarus->setText(settings->value("icarus").toString());
	ui->pathTechnologies->setText(settings->value("tech_path").toString());
}

void Settings::on_selectYosys_pressed()
{
	QString s = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathYosys->setText(s);
}

void Settings::on_selectYosysABC_pressed()
{
	QString s = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathYosysABC->setText(s);
}

void Settings::on_selectGraywolf_pressed()
{
	QString s = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathGraywolf->setText(s);
}

void Settings::on_selectQrouter_pressed()
{
	QString s = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathQRouter->setText(s);
}

void Settings::on_selectIcarus_pressed()
{
	QString s = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathIcarus->setText(s);
}

void Settings::on_selectTechnologies_pressed()
{
	QString s = QFileDialog::getExistingDirectory(this, "Select a file...", QDir::homePath());
	if(s==QString()) return;
	ui->pathTechnologies->setText(s);
}

void Settings::applyEnvironment()
{
	settings->setValue("yosys",ui->pathYosys->displayText());
	settings->setValue("yosys-abc",ui->pathYosysABC->displayText());
	settings->setValue("graywolf",ui->pathGraywolf->displayText());
	settings->setValue("qrouter",ui->pathQRouter->displayText());
	settings->setValue("icarus",ui->pathIcarus->displayText());
	settings->setValue("tech_path",ui->pathTechnologies->displayText());
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
