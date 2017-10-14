#ifndef SYNTHESISOPTIONS_H
#define SYNTHESISOPTIONS_H

#include <QDialog>

#include "ui_synthesisoptions.h"

class SynthesisOptions : public QDialog
{
	Q_OBJECT
public:
	explicit SynthesisOptions(QWidget *parent = nullptr);

signals:

public slots:

private:
	Ui::SynthesisOptions *ui;
};

#endif // SYNTHESISOPTIONS_H
