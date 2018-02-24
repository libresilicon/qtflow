#include "def_editor_app.h"

DefEditor::DefEditor(int &argc, char **argv) :
	QApplication(argc,argv),
	parser(new QCommandLineParser)
{
	parser->setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser->addHelpOption();
	parser->addVersionOption();
	parser->addOption(QCommandLineOption(QStringList() << "f" << "file", QCoreApplication::translate("main", "File to open"),"1"));
	parser->addOption(QCommandLineOption(QStringList() << "l" << "lef", QCoreApplication::translate("main", "LEF file"),"1"));
	parser->process(*this);
}

int DefEditor::exec()
{
	QString filePath;
	DEFLayoutEditorWidget w;
	if(parser->isSet("file")) {
		filePath = parser->value("file");
		qDebug() << "Opening DEF file " << filePath;
		w.show();
		if(parser->isSet("lef")) {
			w.loadLEF(parser->value("lef"));
		}
		w.loadFile(filePath);
		return QApplication::exec();
	} else {
		qDebug() << "No DEF file specified. Doing nothing";
		return 0;
	}
}
