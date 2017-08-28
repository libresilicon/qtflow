#include "synthesistoolbox.h"

SynthesisToolBox::SynthesisToolBox(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::TestBenchToolBox)
{
	ui->setupUi(this);
	setMaximumHeight(ui->dockWidgetContents->height());
}
