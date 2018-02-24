#ifndef SPECIALNETS_H
#define SPECIALNETS_H

#include <QDialog>
#include <QDebug>

#include "project.h"

#include "ui_specialnets.h"

class SpecialNets : public QDialog
{
	Q_OBJECT
public:
	explicit SpecialNets(QWidget *parent = nullptr);
	void setProject(Project *p);

protected:
	void refreshTables();

signals:

public slots:
	void on_buttonBox_accepted();

private:
	Project *project;
	Ui::SpecialNets *ui;
};

#endif // SPECIALNETS_H
