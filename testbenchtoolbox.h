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

class TestBenchToolBox : public QDockWidget
{
	Q_OBJECT
public:
	explicit TestBenchToolBox(QWidget *parent = nullptr);

signals:
	void runSimulation();

public slots:
	void onRunSimulation();

private:
	Ui::TestBenchToolBox *ui;
};

#endif // TESTBENCHTOOLBOX_H
