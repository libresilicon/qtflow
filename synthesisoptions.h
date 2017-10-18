#ifndef SYNTHESISOPTIONS_H
#define SYNTHESISOPTIONS_H

#include <QDialog>
#include <QDebug>

#include "ui_synthesisoptions.h"

#include "project.h"

class SynthesisOptions : public QDialog
{
	Q_OBJECT
public:
	explicit SynthesisOptions(QWidget *parent = nullptr);
	void setProject(Project*p);

signals:

public slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

private:
	Ui::SynthesisOptions *ui;
	Project *project;
};

#endif // SYNTHESISOPTIONS_H
