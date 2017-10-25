#ifndef SCHEMATICSPARAMETRICPARTDIALOG_H
#define SCHEMATICSPARAMETRICPARTDIALOG_H

#include <QDialog>

#include "ui_parametricpart.h"

class SchematicsParametricPartDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SchematicsParametricPartDialog(QWidget *parent = nullptr);

signals:

public slots:

private:
	Ui::ParametricPartDialog *ui;
};

#endif // SCHEMATICSPARAMETRICPARTDIALOG_H
