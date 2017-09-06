#ifndef SYNTHESISTOOLBOX_H
#define SYNTHESISTOOLBOX_H

#include <QToolBar>
#include <QPixmap>
#include <QDockWidget>
#include <QWidget>
#include <QAction>
#include <QToolButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui_synthesistoolbox.h"

class SynthesisToolBox : public QDockWidget
{
	Q_OBJECT
public:
	explicit SynthesisToolBox(QWidget *parent = nullptr);

signals:
	void runSynthesis();

public slots:
	void onRunSynthesis();

private:
	Ui::SynthesisToolBox *ui;
};

#endif // TESTBENCHTOOLBOX_H
