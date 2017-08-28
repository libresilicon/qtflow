#ifndef TESTBENCHTOOLBOX_H
#define TESTBENCHTOOLBOX_H

#include <QToolBar>
#include <QPixmap>
#include <QDockWidget>
#include <QWidget>
#include <QAction>
#include <QToolButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui_testbenchtoolbox.h"

class SynthesisToolBox : public QDockWidget
{
	Q_OBJECT
public:
	explicit SynthesisToolBox(QWidget *parent = nullptr);

signals:

public slots:
private:
	Ui::TestBenchToolBox *ui;
};

#endif // TESTBENCHTOOLBOX_H
