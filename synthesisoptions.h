#ifndef SYNTHESISOPTIONS_H
#define SYNTHESISOPTIONS_H

#include <QWidget>

#include "ui_synthesisoptions.h"

class SynthesisOptions : public QWidget
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
