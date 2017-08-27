#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include "magicparser.h"
#include "templates.h"
//#include "grid.h"
#include "welcome.h"
#include "settings.h"

#include "projectselector.h"
#include "fileselector.h"
#include "moduleselector.h"

#include <iostream>
#include <string>

#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QString>
#include <QProcess>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDockWidget>

MainWindow::MainWindow(QCommandLineParser *p) :
	QMainWindow(NULL),
	parser(p),
	ui(new Ui::MainWindow),
	tcsh(new QProcess),
	welcomeWidget(new Welcome),
	createWidget(new New),
	errorMessage(new QErrorMessage)
{
	ui->setupUi(this);
	project = NULL;

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	settingsDialog = new Settings(this, settings);
	connect(settingsDialog, SIGNAL(syncSettings()), this, SLOT(syncSettings()));

	iopadsWidget = new IOPads(this);
	iopadsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, iopadsWidget);

	filesWidget = new FileSelector(this);
	filesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, filesWidget);

	projectsWidget = new ProjectSelector(this);
	projectsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, projectsWidget);

	modulesWidget = new ModuleSelector(this);
	modulesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, modulesWidget);
	connect(modulesWidget, SIGNAL(setTopLevel(QString)), this, SLOT(setTopLevel(QString)));
	connect(modulesWidget, SIGNAL(setTestBench(QString)), this, SLOT(setTestBench(QString)));

	timingWidget = new Wave(this);
	timingWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, timingWidget);

	editArea = new EditorTabManager(ui->frame);
	connect(filesWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(projectsWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(editArea,SIGNAL(fileSaved()),modulesWidget,SLOT(refresh()));

	mainToolBox = new MainToolBox(this);
	mainToolBox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::TopDockWidgetArea, mainToolBox);

	connect(tcsh, SIGNAL(readyReadStandardOutput()), this, SLOT(fireTcsh()));
	connect(tcsh, SIGNAL(readyReadStandardError()), this, SLOT(errorTcsh()));
	connect(tcsh, SIGNAL(finished(int)), this, SLOT(exitTcsh(int)));

	QMenu *recent = ui->menuRecentProjects;
	QAction *recent_action;

	settings->beginGroup("history");
	QStringList recentProjectsList = settings->value("recentProjects").toStringList();
	settings->endGroup();

	foreach(QString recentProject, recentProjectsList) {
		recent_action=recent->addAction(recentProject);
		recent_action->setData(recentProject);
		connect(recent_action, SIGNAL(triggered()), this, SLOT(openRecentProject()));
	}

	hideAllDockerWidgets();

	if(parser) {
		if(parser->isSet("top-level")) {
			openProject(QDir(".").absolutePath()+"/"+parser->value("top-level")+".pro");
		}
	}
}

void MainWindow::hideAllDockerWidgets()
{
	filesWidget->setVisible(false);
	projectsWidget->setVisible(false);
	modulesWidget->setVisible(false);
	timingWidget->setVisible(false);
	mainToolBox->setVisible(false);
	iopadsWidget->setVisible(false);
}

void MainWindow::openProject(QString path)
{
	QFile project_vars(path);
	if (project_vars.exists()) {
		if(project) delete project;
		project = new Project(settings, path);
		modulesWidget->setProject(project);

		filesWidget->setSourceDir(project->getSourceDir());
		projectsWidget->setRootDir(project->getRootDir());
		enableProject();
	}
}

void MainWindow::openRecentProject()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if(action) openProject(action->data().toString());
}

void MainWindow::syncSettings()
{
	settings->sync();
}

MainWindow::~MainWindow()
{
	delete ui;
	delete errorMessage;
	if(project) delete project;
	//delete dependencies;
	delete createWidget;
	delete welcomeWidget;
	delete timingWidget;
	delete iopadsWidget;
	delete tcsh;
}

void MainWindow::on_MainWindow_destroyed()
{

}

void MainWindow::on_newProject_triggered()
{
	Templates *t = new Templates(this);
	t->show();
}

void MainWindow::on_menuSettings_triggered()
{
	settingsDialog->show();
}

void MainWindow::on_openProject_triggered()
{
	QString filter = "File Description (*.pro)";
	QString path = QFileDialog::getOpenFileName(this, "Select a file...", QDir::homePath(), filter);
	openProject(path);
}

void MainWindow::on_newFile_triggered()
{
	createWidget->suggest(Verilog, "new");
	createWidget->show();
}

void MainWindow::on_saveFile_triggered()
{
}

void MainWindow::on_openMagicFile_triggered()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Magic Files (*.mag)"));

	if (name == QString())
		return;

	qDebug() << "Open magic file:" << name;
	/*QFile file(name);
	file.open(QFile::ReadOnly);
	QByteArray content(file.readAll());
	MagicParser parser(content);
	rects_t grid = parser.getRectangles();
	Grid *g = new Grid();
	g->show();
	g->RenderRectangles(grid);*/
}

void MainWindow::on_exit_triggered()
{
	//close();
}

void MainWindow::on_buildAll_triggered()
{
	if (tcsh->state() == QProcess::Running)
		return;

	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
	//QString path = session.project();
	//QSettings env(path);
	//tcsh->setWorkingDirectory(path);
	//project->buildAll(env.value(DEFAULT_VERILOG), tcsh);
}

void MainWindow::on_buildVcd_triggered()
{
	if (tcsh->state() == QProcess::Running)
		return;

	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
	//QString path = session.project();
	//ProjectSettings env(path);
	//tcsh->setWorkingDirectory(path);
	//project->valuedump(env.value(DEFAULT_VERILOG), tcsh);
}

void MainWindow::on_menuSynthesis_triggered()
{
	if (tcsh->state() == QProcess::Running)
		return;

	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
	//QString path = session.project();
	//ProjectSettings env(path);
	//tcsh->setWorkingDirectory(path);
	//project->synthesis(env.value(DEFAULT_VERILOG), tcsh);
}

void MainWindow::on_menuPlacement_triggered()
{
	if (tcsh->state() == QProcess::Running)
		return;

	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
	//QString path = session.project();
	//ProjectSettings env(path);
	//tcsh->setWorkingDirectory(path);
	//project->placement(env.value(DEFAULT_VERILOG), tcsh);
}

void MainWindow::on_menuRouting_triggered()
{
	if (tcsh->state() == QProcess::Running)
		return;

	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
	//QString path = session.project();
	//ProjectSettings env(path);
	//tcsh->setWorkingDirectory(path);
	//project->routing(env.value(DEFAULT_VERILOG), tcsh);
}

void MainWindow::on_menuModules_triggered()
{
	//modules->show();
	//modules->refresh();
}

void MainWindow::on_menuIOPads_triggered()
{
	iopadsWidget->show();
}

void MainWindow::on_toolRefresh_triggered()
{
	enableProject();
}

void MainWindow::fireTcsh()
{
	QByteArray bytes = tcsh->read(4096);
	//ui->consoleOut->insertPlainText(bytes);
	//ui->consoleOut->verticalScrollBar()->setValue(ui->consoleOut->verticalScrollBar()->maximum());
}

void MainWindow::errorTcsh()
{
	QByteArray bytes = tcsh->readAllStandardError();
	//ui->consoleError->insertPlainText(bytes);
	//ui->consoleError->verticalScrollBar()->setValue(ui->consoleError->verticalScrollBar()->maximum());
}

void MainWindow::exitTcsh(int code)
{
	qDebug() << "Tcsh exited with code" << code;
	ui->menuSynthesis->setDisabled(false);
	ui->menuPlacement->setDisabled(false);
	ui->menuRouting->setDisabled(false);
	ui->buildAll->setDisabled(false);
	enableTopModule();
}

void MainWindow::onTopModuleChanged()
{
	enableTopModule();
}

void MainWindow::enableProject()
{
	enableTopModule();
	//ui->tabWidget->show();

	ui->newFile->setDisabled(false);
	ui->buildAll->setDisabled(false);
	ui->buildSteps->setDisabled(false);
	ui->buildEnvironment->setDisabled(false);
	ui->buildVcd->setDisabled(false);
	ui->menuSynthesis->setDisabled(false);
	ui->menuPlacement->setDisabled(false);
	ui->menuRouting->setDisabled(false);
	ui->menuModules->setDisabled(false);
	ui->toolRefresh->setDisabled(false);

	filesWidget->setVisible(1);
	projectsWidget->setVisible(1);
	modulesWidget->setVisible(1);
}

void MainWindow::disableProject()
{
	ui->newFile->setDisabled(true);
	ui->buildAll->setDisabled(true);
	ui->buildSteps->setDisabled(true);
	ui->buildEnvironment->setDisabled(true);
	ui->buildVcd->setDisabled(true);
	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->menuModules->setDisabled(true);
	ui->toolRefresh->setDisabled(true);
}

void MainWindow::enableTopModule()
{
	QString path = project->getSourceDir()+"/"+project->getTopLevel()+".vcd";
	QFile file(path);

	if (!file.exists())
	{
		timingWidget->setDisabled(true);
		return;
	}
	timingWidget->loadVcd(path);
	timingWidget->setDisabled(false);
}

void MainWindow::setTopLevel(QString name)
{
	project->setTopLevel(name);
}

void MainWindow::setTestBench(QString name)
{
	project->setTestBench(name);
}

void MainWindow::enableFile()
{
	ui->saveFile->setDisabled(false);
}

void MainWindow::disableFile()
{
	ui->saveFile->setDisabled(true);
}

void MainWindow::error(QString e)
{
	errorMessage->showMessage(e);
	errorMessage->show();
}
