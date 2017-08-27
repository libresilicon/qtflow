#include "toolboxbasis.h"

ToolBoxBasis::ToolBoxBasis(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::ToolBox)
{
	ui->setupUi(this);
	ui->frame->setMinimumHeight(ICON_SIZE);
	ui->frame->setMinimumWidth(ICON_SIZE);
}

QToolButton *ToolBoxBasis::addButton(QString image)
{
	QWidget *buttonWidget = new QWidget(ui->frame);
	QToolButton *button = new QToolButton(buttonWidget);
	button->setIcon(QIcon(":/media-playback-start.svg"));
	button->setMinimumSize(ICON_SIZE,ICON_SIZE);
	button->setMaximumSize(ICON_SIZE,ICON_SIZE);
	button->setIconSize(button->size());

	return button;
}
