#include "magic_editor_app.h"

MagicEditor::MagicEditor(int &argc, char **argv) :
	QApplication(argc,argv),
	parser(new QCommandLineParser)
{
	parser->setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser->addHelpOption();
	parser->addVersionOption();
	parser->addOption(QCommandLineOption(QStringList() << "f" << "file", QCoreApplication::translate("main", "File to open"),"1"));
	parser->addOption(QCommandLineOption(QStringList() << "l" << "lef", QCoreApplication::translate("main", "LEF file"),"1"));
	parser->addOption(QCommandLineOption(QStringList() << "t" << "technology", QCoreApplication::translate("main", "Technology short name"),"1"));
	parser->process(*this);
}

int MagicEditor::exec()
{
	QString tech_name;
	QString filePath;
	MagicLayoutEditorWidget w;
	if(parser->isSet("file")) {
		filePath = parser->value("file");
		qDebug() << "Opening Magic file " << filePath;
		w.show();
		if(parser->isSet("lef")) {
			w.loadLEF(parser->value("lef"));
		}
		tech_name="ls1u";
		if(parser->isSet("technology")) {
			QFileInfo fi(":/"+parser->value("technology")+".toml");
			if(fi.exists()) {
				tech_name = parser->value("technology");
			} else {
				qDebug() << "Technology: " << parser->value("technology") << " doesn't exist, using technology " << tech_name;
			}
		}
		qDebug() << "Using technology: " << tech_name;
		TechDataWrapper toml(":/"+tech_name+".toml");
		w.setTechnologyData(&toml);
		w.setLambdaUnit(toml.getLambdaUnit());
		w.setLambdaValue(toml.getLambdaValue());
		w.loadFile(filePath);
		return QApplication::exec();
	} else {
		qDebug() << "No Magic file specified. Doing nothing";
		return 0;
	}
}
