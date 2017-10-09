#ifndef SCHEMATICSPARTSELECTION_H
#define SCHEMATICSPARTSELECTION_H

#include <QObject>
#include <QWidget>

#include "ui_partselection.h"

class SchematicsPartSelection : public QWidget
{
public:
	SchematicsPartSelection(QWidget *parent);

private:
	Ui::PartSelection *ui;
};

#endif // SCHEMATICSPARTSELECTION_H
