#include "synthesistoolbox.h"

SynthesisToolBox::SynthesisToolBox(QWidget *parent) :
	QDockWidget(parent),
	ui(new Ui::SynthesisToolBox)
{
	ui->setupUi(this);
	setMaximumHeight(ui->dockWidgetContents->height()+20);
}

void SynthesisToolBox::onRunSynthesis()
{
	emit(runSynthesis());
}
