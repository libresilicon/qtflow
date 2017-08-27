#include "testbenchtoolbox.h"

TestBenchToolBox::TestBenchToolBox(QWidget *parent) :
	ToolBoxBasis(parent)
{
	QToolButton *startButton = addButton(":/media-playback-start.svg");
	QToolButton *stopButton = addButton(":/media-playback-stop.svg");
}
