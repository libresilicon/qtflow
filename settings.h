#ifndef OPTIONS_H
#define OPTIONS_H

#include "ui_options.h"

#include <QWidget>
#include <QDialog>

class Options : public QDialog
{
	Q_OBJECT

public:
	explicit Options(QDialog *parent = nullptr);

signals:
	

public slots:
	
private:
	Ui::Options *ui;

};

#endif // OPTIONS_H
