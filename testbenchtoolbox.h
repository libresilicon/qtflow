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

#include "toolboxbasis.h"

class TestBenchToolBox : public ToolBoxBasis
{
	Q_OBJECT
public:
	explicit TestBenchToolBox(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // TESTBENCHTOOLBOX_H
