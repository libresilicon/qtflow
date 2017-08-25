#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lexmagic.h"
#include "magicparser.h"
#include "templates.h"
#include "grid.h"
#include "welcome.h"
#include "settings.h"

#include "editor.h"
#include "verilog.h"

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
	//ui->consoleError->hide();
	//ui->tabWidget->tabBar()->hide();
	//ui->tabWidget->insertTab(0, welcomeWidget, "Welcome");
	//ui->tabWidget->insertTab(2, timingWidget, "Timing");
	//ui->tabWidget->insertTab(3, new QWidget, "Design");

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	settingsDialog = new Settings(this, settings);
	connect(settingsDialog, SIGNAL(syncSettings()), this, SLOT(syncSettings()));

	//iopads = new IOPads(this);
	//iopads->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	//addDockWidget(Qt::RightDockWidgetArea, iopads);

	filesWidget = new FileSelector(this);
	filesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, filesWidget);
	connect(filesWidget, SIGNAL(openFile(QString)), this, SLOT(openFile(QString)));

	projectsWidget = new ProjectSelector(this);
	projectsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, projectsWidget);

	modulesWidget = new ModuleSelector(this);
	modulesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, modulesWidget);
	connect(modulesWidget, SIGNAL(setTopLevel(QString)), this, SLOT(setTopLevel(QString)));

	timingWidget = new Wave(this);
	timingWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, timingWidget);

	editArea = new QTabWidget(ui->frame);
	editArea->resize(ui->frame->maximumSize());
	editArea->setTabsClosable(true);
	connect(editArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
	connect(editArea, SIGNAL(currentChanged(int)), this, SLOT(showEditDockerWidgets(int)));

	editorToolBar = new EditorToolBar(this);
	editorToolBar->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::TopDockWidgetArea, editorToolBar);

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
	editorToolBar->setVisible(false);
}

void MainWindow::showEditDockerWidgets(int i)
{
		editorToolBar->setVisible((editArea->count()>0)?true:false);
}

void MainWindow::openProject(QString path)
{
	QFile project_vars(path);
	if (project_vars.exists()) {
		if(project) delete project;
		project = new Project(settings, path);
		modulesWidget->setSourceDir(project->getSourceDir());
		filesWidget->setSourceDir(project->getSourceDir());
		projectsWidget->setSourceDir(project->getSourceDir());
		enableProject();
	}
}

bool MainWindow::isCode(QString suffix)
{
	if(suffix=="v") return true;
	if(suffix=="vs") return true;
	return false;
}

bool MainWindow::isSchematic(QString suffix)
{
	return false;
}

void MainWindow::openFile(QString file)
{
	QWidget *tabArea;
	QVBoxLayout *layout;
	QString filepath = project->getSourceDir()+'/'+file;
	QFileInfo info(filepath);

	for(int idx=0; idx<editArea->count(); idx++) {
		Editor *ed = (Editor *)editArea->widget(idx);
		if(ed->getFilePath()==filepath) return; // already open
	}

	if(isCode(info.suffix())||isSchematic(info.suffix())) {
		tabArea = new QWidget(editArea);
		layout = new QVBoxLayout(tabArea);
	}

	if(isCode(info.suffix())) {
		QToolBar *toolbar = new QToolBar("Main toolbar", tabArea);
		QPixmap pixmapDocumentSave(":/document-save.svg");
		QAction *saveButton = new QAction(pixmapDocumentSave,"S&ave", toolbar);
		toolbar->addAction(saveButton);
		layout->setMenuBar(toolbar);

		Editor *editorWidget = new Editor(tabArea);
		editorWidget->loadFile(filepath);
		if(info.suffix()=="v") {
			editorWidget->setSyntax(new VerilogHighlight(editorWidget->document()));
		}
		layout->addWidget(editorWidget);

		//connect(editorWidget,SIGNAL(textChanged()),tabArea,SLOT(setWindowModified(bool)));

		tabArea->setLayout(layout);
		editArea->addTab(tabArea,file);
	}
}

void MainWindow::closeFile(int idx)
{
	//Editor *ed = (Editor*)editArea->widget(idx);
	//ed->saveFile();
	editArea->removeTab(idx);
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
