#include "drcsettings.h"

DRCSettings::DRCSettings(QWidget *parent):
	QDialog(parent),
	ui(new Ui::DRCSettings),
	project(NULL)
{
	ui->setupUi(this);
}

void DRCSettings::setProject(Project *p)
{
	project = p;
	refreshRuleList();
}


void DRCSettings::refreshRuleList()
{
	TechDesignRule rule;
	QString designRuleName;
	QString longName;
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
		ui->tableWidths->setRowCount(i+1);
		ui->tableWidths->setItem(i, 0, new QTableWidgetItem(designRuleName));
		i++;
	}

	header.clear();
	header << "Layer name";
	header << "to material";
	header << "spacing";

	ui->tableSpacings->clear();
	ui->tableSpacings->setColumnCount(header.count());
	ui->tableSpacings->setHorizontalHeaderLabels(header);
}
