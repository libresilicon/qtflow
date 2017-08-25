#include "editortoolbar.h"

EditorToolBar::EditorToolBar(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::EditorToolBar)
{
	ui->setupUi(this);
}
