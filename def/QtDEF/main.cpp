#include "qtdef.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtDEF w;
	w.show();

	return a.exec();
}
