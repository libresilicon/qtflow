#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include "magiclayouteditorwidget.h"

class MagicEditor : public QApplication {
	Q_OBJECT

	QCommandLineParser *parser;
public:
	MagicEditor(int &argc, char **argv);
	int exec();
};
