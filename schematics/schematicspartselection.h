#ifndef SCHEMATICSPARTSELECTION_H
#define SCHEMATICSPARTSELECTION_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QDebug>

#include "ui_partselection.h"

#include "lef/lefdata.h"

class SchematicsPartSelection : public QDialog
{
	Q_OBJECT

public:
	SchematicsPartSelection(QWidget *parent);

	void setLEF(lef::LEFData *d);

public slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	void on_partList_currentTextChanged(QString);

private:
	void updatePartList();

	Ui::PartSelection *ui;
	lef::LEFData *lefdata;
};

#endif // SCHEMATICSPARTSELECTION_H
