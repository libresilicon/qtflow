#include "drcsettings.h"

DRCSettings::DRCSettings(QWidget *parent):
	QDialog(parent),
	ui(new Ui::DRCSettings),
	project(NULL)
{
	ui->setupUi(this);
}

void DRCSettings::on_buttonBox_accepted()
{
	qDebug() << ui->checkBoxRealTimeCheck->isChecked();
}

void DRCSettings::on_runDRC_pressed()
{
	emit(runDRC());
}

void DRCSettings::setProject(Project *p)
{
	project = p;
	refreshRuleList();
}

void DRCSettings::refreshRuleList()
{
	TechDesignRule rule;
	TechDesignRuleSpacing spacing;
	QString designRuleName;
	QStringList header;
	int i;

	header.clear();
	header << "Layer name";
	header << "Minimum width";

	ui->tableWidths->clear();
	ui->tableWidths->setColumnCount(header.count());
	ui->tableWidths->setHorizontalHeaderLabels(header);

	ui->tableWidths->setRowCount(10);
	ui->tableWidths->setItem(1, 0, new QTableWidgetItem("Hello"));

	i=0;
	if(project) foreach(designRuleName, project->getDesignRules()) {
		rule = project->getDesignRule(designRuleName);
		if(rule.getMinimumWidth()) {
			ui->tableWidths->setRowCount(i+1);
			ui->tableWidths->setItem(i, 0, new QTableWidgetItem(project->getLongName(rule.getName())));
			ui->tableWidths->setItem(i, 1, new QTableWidgetItem(QString::number(rule.getMinimumWidth())));
			i++; // next line
		}
	}

	header.clear();
	header << "Layer name";
	header << "to material";
	header << "spacing";
	header << "touching ok";

	ui->tableSpacings->clear();
	ui->tableSpacings->setColumnCount(header.count());
	ui->tableSpacings->setHorizontalHeaderLabels(header);

	i=0;
	if(project) foreach(designRuleName, project->getDesignRules()) {
		rule = project->getDesignRule(designRuleName);
		foreach(QString sprn, rule.getSpacingRules()) {
			spacing = rule.getSpacingRule(sprn);
			ui->tableSpacings->setRowCount(i+1);
			ui->tableSpacings->setItem(i, 0, new QTableWidgetItem(project->getLongName(designRuleName)));
			ui->tableSpacings->setItem(i, 1, new QTableWidgetItem(project->getLongName(spacing.getMaterial())));
			ui->tableSpacings->setItem(i, 2, new QTableWidgetItem(QString::number(spacing.getSpacing())));
			ui->tableSpacings->setItem(i, 3, new QTableWidgetItem(spacing.getTouchingOK()?"Yes":"No"));
			i++; // next line
		}
	}

}
