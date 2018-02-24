#ifndef DRCSETTINGS_H
#define DRCSETTINGS_H

#include <QDialog>
#include <QDebug>

#include "ui_drcsettings.h"
#include "tech_reader/techdesignrule.h"

class DRCSettings : public QDialog
{
	Q_OBJECT
public:
	DRCSettings(QWidget *parent = Q_NULLPTR);

public slots:
	void on_buttonBox_accepted();
	void on_runDRC_pressed();

signals:
	void runDRC();

private:
	void refreshRuleList();

	Ui::DRCSettings *ui;
};

#endif // DRCSETTINGS_H
