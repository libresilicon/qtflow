#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lexmagic.h"
#include "magicparser.h"
#include "templates.h"
#include "grid.h"
#include "edit.h"
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

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	tcsh(new QProcess),
	welcomeWidget(new Welcome),
	timingWidget(new Wave),
	createWidget(new New),
	errorMessage(new QErrorMessage),
	filesystem(new QFileSystemModel),
	projects(new ProjectsTreeModel(this))
{
	ui->setupUi(this);
	project = NULL;
	//ui->consoleError->hide();
	//ui->tabWidget->tabBar()->hide();
	//ui->tabWidget->insertTab(0, welcomeWidget, "Welcome");
	//ui->tabWidget->insertTab(2, timingWidget, "Timing");
	//ui->tabWidget->insertTab(3, new QWidget, "Design");

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, ".qtflow", ".qtflow");
	settingsDialog = new Settings(this, settings);

	//editWidget = new Edit(this);
	//editWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	//addDockWidget(Qt::LeftDockWidgetArea, editWidget);

	//iopads = new IOPads(this);
	//iopads->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	//addDockWidget(Qt::RightDockWidgetArea, iopads);

	filesWidget = new FileSelector(this);
	filesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, filesWidget);

	projectsWidget = new ProjectSelector(this);
	projectsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, projectsWidget);

	modulesWidget = new ModuleSelector(this);
	modulesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, modulesWidget);

	connect(tcsh, SIGNAL(readyReadStandardOutput()), this, SLOT(fireTcsh()));
	connect(tcsh, SIGNAL(readyReadStandardError()), this, SLOT(errorTcsh()));
	connect(tcsh, SIGNAL(finished(int)), this, SLOT(exitTcsh(int)));

	//connect(createWidget, SIGNAL(fileCreated(QFileInfo&)), editWidget, SLOT(onLoadFile(QFileInfo&)));

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
}

void MainWindow::openRecentProject()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if(action) {
		QString path = action->data().toString();
		QFile project_vars(path);
		if (project_vars.exists()) {
			if(project) delete project;
			project = new Project(path);
			modulesWidget->setSourceDir(project->getSourceDir());
			modulesWidget->refresh();
		}
	}
}

MainWindow::~MainWindow()
{
	delete ui;
	delete errorMessage;
	if(project) delete project;
	//delete dependencies;
	delete createWidget;
	delete welcomeWidget;
	delete editWidget;
	delete timingWidget;
	delete iopads;
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

	QFile project_vars(path);
	if (project_vars.exists())
	{
		if(project) delete project;
		project = new Project(path);
		modulesWidget->setSourceDir(project->getSourceDir());
		modulesWidget->refresh();
		enableProject();
	}
}

void MainWindow::on_newFile_triggered()
{
	createWidget->suggest(Verilog, "new");
	createWidget->show();
}

void MainWindow::on_saveFile_triggered()
{
	//editWidget->saveFile(session.file());
}

void MainWindow::on_openMagicFile_triggered()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Magic Files (*.mag)"));

	if (name == QString())
		return;

	qDebug() << "Open magic file:" << name;
	QFile file(name);
	file.open(QFile::ReadOnly);
	QByteArray content(file.readAll());
	MagicParser parser(content);
	rects_t grid = parser.getRectangles();
	Grid *g = new Grid();
	g->show();
	g->RenderRectangles(grid);
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
	iopads->show();
}

void MainWindow::on_toolRefresh_triggered()
{
	enableProject();
}

void MainWindow::on_mainWelcome_clicked()
{
	//ui->tabWidget->show();
	//ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_mainEdit_clicked()
{
	//ui->tabWidget->show();
	//ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_mainTiming_clicked()
{
	//ui->tabWidget->show();
	//ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_tcshExpand_clicked()
{
	//ui->tabWidget->hide();
	//ui->consoleOut->show();
	//ui->consoleError->hide();
}

void MainWindow::on_tcshErrors_clicked()
{
	//ui->tabWidget->hide();
	//ui->consoleOut->hide();
	//ui->consoleError->show();
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
	//ui->mainEdit->setDisabled(false);
}

void MainWindow::disableProject()
{
	//ui->tabWidget->setCurrentIndex(0);
	//ui->tabWidget->show();

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
	//ui->mainEdit->setDisabled(true);
	//ui->mainDesign->setDisabled(true);
}

void MainWindow::enableTopModule()
{
	//ProjectSettings env(session.project());
	//QString path = env.value("sourcedir") + "/" + env.value(DEFAULT_VERILOG) + ".vcd";
	//QFile file(path);

	//if (!file.exists())
	//{
	//	ui->mainTiming->setDisabled(true);
	//	return;
	//}

	//timingWidget->loadVcd(path);
	//ui->mainTiming->setDisabled(false);
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
