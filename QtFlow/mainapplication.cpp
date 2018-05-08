#include "mainapplication.h"

MainApplication::MainApplication(int &argc, char **argv):
	QApplication(argc,argv),
	parser(new QCommandLineParser()),
	settingsFileProcess(new QDomDocument())
{
}

int MainApplication::runHeadless()
{
	QString path = QDir(".").absolutePath();
	QString tech;
	QString proc;

	console = new PythonQtScriptingConsole(NULL, mainContext);
	if(parser->isSet("console")) {
		console->show();
	}

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	qDebug() << "Getting config file " << settings->fileName();

	mainContext.addObject("settings", new PySettings(this, settings));

	if(parser->isSet("top-level")) {
		project = new Project(settings, QDir(path).filePath(parser->value("top-level")+".pro"), mainContext);
		project->setTopLevel(parser->value("top-level"));
	} else {
		qErrnoWarning("No top level given!");
		return 0;
	}

	QFile file(QDir(project->getTechPath()).filePath("process.xml"));
	if(file.open(QIODevice::ReadOnly)) {
		settingsFileProcess->setContent(&file);
		file.close();
	}

	if(parser->isSet("technology")) {
		tech = parser->value("technology");
		if(isValidTechnology(tech)) {
			project->setTechnology(tech);
		} else {
			qDebug() << "Invalid technology!";
			return handleExit();
		}
	}
	qDebug() << "Using technology: " << project->getTechnology();

	if(parser->isSet("process")) {
		proc = parser->value("process");
		if(isValidProcess(proc)) {
			project->setProcess(proc);
		} else {
			qDebug() << "Invalid process!";
			return handleExit();
		}
	}
	qDebug() << "Using process: " << project->getProcess();

	mainContext.addObject("project_settings", new PyProjectSettings(project));

	if (parser->isSet("run-synthesis")||parser->isSet("run-placement")||parser->isSet("run-routing")){
		if(parser->isSet("run-synthesis")) project->synthesis();
		if(parser->isSet("run-placement")) project->placement();
		if(parser->isSet("run-routing")) project->routing();
	} else {
		project->buildAll();
	}

	return handleExit();
}

int MainApplication::handleExit()
{
	if(parser->isSet("console")) {
		return QApplication::exec();
	} else {
		return 0;
	}
}

int MainApplication::exec()
{
	PythonQt::init();
	mainContext = PythonQt::self()->getMainModule();

	parser->setApplicationDescription("QtFlow: Qt5 based synthesis tool");
	parser->addHelpOption();
	parser->addVersionOption();
	parser->addOption(QCommandLineOption(QStringList() << "n" << "no-graphics", QCoreApplication::translate("main", "Run without graphics")));

	// what is the main module:
	parser->addOption(QCommandLineOption(QStringList() << "m" << "top-level", QCoreApplication::translate("main", "Top level module"),"1"));

	// build steps:
	parser->addOption(QCommandLineOption(QStringList() << "s" << "run-synthesis", QCoreApplication::translate("main", "Run synthesis of module")));
	parser->addOption(QCommandLineOption(QStringList() << "p" << "run-placement", QCoreApplication::translate("main", "Run placement of module")));
	parser->addOption(QCommandLineOption(QStringList() << "r" << "run-routing", QCoreApplication::translate("main", "Run routing of module")));

	// process and technology:
	parser->addOption(QCommandLineOption(QStringList() << "T" << "technology", QCoreApplication::translate("main", "Use specific technology"),"1"));
	parser->addOption(QCommandLineOption(QStringList() << "P" << "process", QCoreApplication::translate("main", "Use specific process"),"1"));

	// semi interactivity:
	parser->addOption(QCommandLineOption(QStringList() << "c" << "console", QCoreApplication::translate("main", "Show Python console")));

	parser->process(*this);

	if(parser->isSet("no-graphics")) {
		return runHeadless();
	} else {
		w = new MainWindow(parser, mainContext);
		w->showMaximized();
		return QApplication::exec();
	}
}

bool MainApplication::isValidTechnology(QString technology)
{
	QDomElement e;
	QDomNodeList nl;

	nl = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl.count(); i++) {
		e = nl.at(i).toElement();
		if(e.attribute("xml:id")==technology) {
			return true;
		}
	}
	return false;
}

bool MainApplication::isValidProcess(QString process)
{
	QDomElement e1, e2;
	QDomNodeList nl1, nl2;

	nl1 = settingsFileProcess->elementsByTagName("technology");
	for(int i = 0; i< nl1.count(); i++) {
		e1 = nl1.at(i).toElement();
		if(e1.attribute("xml:id")==project->getTechnology()) {
			nl2 = e1.childNodes();
			for(int j = 0; j < nl2.count(); j++) {
				e2 = nl2.at(j).toElement();
				if(e2.tagName()=="process") {
					if(e2.attribute("xml:id")==process) {
						return true;
					}
				}
			}
		}
	}
	return false;
}
