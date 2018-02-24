#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include "deflayouteditorwidget.h"

class DefEditor : public QApplication {
	Q_OBJECT

	QCommandLineParser *parser;
public:
	DefEditor(int &argc, char **argv);
	int exec();
};
