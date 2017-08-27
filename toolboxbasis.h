#ifndef TOOLBOXBASIS_H
#define TOOLBOXBASIS_H

#include <QToolBar>
#include <QPixmap>
#include <QDockWidget>
#include <QWidget>
#include <QAction>
#include <QToolButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ui_toolbox.h"

#define ICON_SIZE 64

class ToolBoxBasis : public QDockWidget
{
	Q_OBJECT
public:
	explicit ToolBoxBasis(QWidget *parent = nullptr);
	QToolButton *addButton(QString image);

signals:

public slots:
private:
	Ui::ToolBox *ui;
};

#endif // TOOLBOXBASIS_H
