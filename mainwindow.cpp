#include "mainwindow.h"

MainWindow::MainWindow(QCommandLineParser *p, PythonQtObjectPtr *context ) :
	QMainWindow(NULL),
	parser(p),
	ui(new Ui::MainWindow),
	welcomeWidget(new Welcome),
	createWidget(new New),
	errorMessage(new QErrorMessage),
	project(NULL),
	mainContext(context)
{
	ui->setupUi(this);

	connect(ui->setAnalogSimulationMode,SIGNAL(triggered(bool)),this,SLOT(on_analogSimulationMode_triggered()));
	connect(ui->setDigialSimulationMode,SIGNAL(triggered(bool)),this,SLOT(on_digitalSimulationMode_triggered()));
	connect(ui->setSynthesisMode,SIGNAL(triggered(bool)),this,SLOT(on_synthesisMode_triggered()));

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	settingsDialog = new Settings(this, settings);
	connect(settingsDialog, SIGNAL(syncSettings()), this, SLOT(syncSettings()));
	mainContext->addObject("settings", new PySettings(this, settings));

	projectSettingsDialog = new ProjectSettings(this);
	connect(ui->projectSettings, SIGNAL(triggered(bool)), projectSettingsDialog, SLOT(open()));

	iopadsWidget = new IOPads(this);
	iopadsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, iopadsWidget);

	filesWidget = new FileSelector(this);
	filesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, filesWidget);

	projectsWidget = new ProjectSelector(this);
	projectsWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, projectsWidget);

	toolBoxWidgetTestBench = new TestBenchToolBox(this);
	toolBoxWidgetTestBench->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, toolBoxWidgetTestBench);
	connect(toolBoxWidgetTestBench,SIGNAL(runSimulation()),this,SLOT(on_menuSimulation_triggered()));

	toolBoxWidgetSynthesis = new SynthesisToolBox(this);
	toolBoxWidgetSynthesis->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	addDockWidget(Qt::RightDockWidgetArea, toolBoxWidgetSynthesis);
	connect(toolBoxWidgetSynthesis,SIGNAL(runSynthesis()),this,SLOT(on_menuSynthesis_triggered()));

	modulesWidget = new ModuleSelector(this);
	modulesWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::LeftDockWidgetArea, modulesWidget);
	connect(modulesWidget, SIGNAL(setTopLevel(QString)), this, SLOT(setTopLevel(QString)));
	connect(modulesWidget, SIGNAL(setTestBench(QString)), this, SLOT(setTestBench(QString)));
	connect(modulesWidget, SIGNAL(setTestBench(QString)), filesWidget, SLOT(refresh()));
	connect(modulesWidget, SIGNAL(setTopLevel(QString)), filesWidget, SLOT(refresh()));

	timingWidget = new Wave(this);
	timingWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::BottomDockWidgetArea, timingWidget);

	pythonConsoleWidget = new PythonConsoleDockWidget(this, mainContext);
	pythonConsoleWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::BottomDockWidgetArea, pythonConsoleWidget);

	editArea = new EditorTabManager(ui->centralWidget);
	connect(filesWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(projectsWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(editArea,SIGNAL(fileSaved()),modulesWidget,SLOT(refresh()));

	mainToolBox = new MainToolBox(this);
	mainToolBox->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::TopDockWidgetArea, mainToolBox);

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
	disableAllFunctions();

	if(parser) {
		if(parser->isSet("top-level")) {
			openProject(QDir(".").absolutePath()+"/"+parser->value("top-level")+".pro");
		}
	}
}

void MainWindow::disableAllFunctions()
{
	ui->setLayoutMode->setEnabled(false);
	ui->setDigialSimulationMode->setEnabled(false);
	ui->setAnalogSimulationMode->setEnabled(false);
	ui->setSynthesisMode->setEnabled(false);
	ui->projectSettings->setEnabled(false);
}

void MainWindow::hideAllDockerWidgets()
{
	filesWidget->setVisible(false);
	projectsWidget->setVisible(false);
	modulesWidget->setVisible(false);
	timingWidget->setVisible(false);
	mainToolBox->setVisible(false);
	iopadsWidget->setVisible(false);
	toolBoxWidgetTestBench->setVisible(false);
	toolBoxWidgetSynthesis->setVisible(false);
	pythonConsoleWidget->setVisible(false);
}

void MainWindow::openProject(QString path)
{
	QFile project_path(path);
	if (project_path.exists()) {
		if(project) delete project;
		project = new Project(settings, path, mainContext);
		modulesWidget->setProject(project);
		filesWidget->setProject(project);
		projectsWidget->setProject(project);
		projectSettingsDialog->setProject(project);
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
}

void MainWindow::on_MainWindow_destroyed()
{
}

void MainWindow::on_digitalSimulationMode_triggered()
{
	hideAllDockerWidgets();
	toolBoxWidgetTestBench->setVisible(true);
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
}

void MainWindow::on_analogSimulationMode_triggered()
{
	hideAllDockerWidgets();
	//toolBoxWidgetTestBench->setVisible(true);
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
}

void MainWindow::on_synthesisMode_triggered()
{
	hideAllDockerWidgets();
	toolBoxWidgetSynthesis->setVisible(true);
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
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

void MainWindow::on_exit_triggered()
{
	//close();
}

void MainWindow::on_buildAll_triggered()
{
	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
}

void MainWindow::on_menuSimulation_triggered()
{
	if(project) {
		project->simulation();
		timingWidget->loadVcd(project->getVCDPath());
		timingWidget->setEnabled(true);
		timingWidget->setVisible(true);
	}
}

void MainWindow::on_menuSynthesis_triggered()
{
	if(project) {
		project->synthesis();
	}
}

void MainWindow::on_menuPlacement_triggered()
{
	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
}

void MainWindow::on_menuRouting_triggered()
{
	ui->menuSynthesis->setDisabled(true);
	ui->menuPlacement->setDisabled(true);
	ui->menuRouting->setDisabled(true);
	ui->buildAll->setDisabled(true);
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

void MainWindow::enableProject()
{
	if(!project) return;

	disableAllFunctions();
	if(project->getProjectType()=="asic_mixed" || project->getProjectType()=="asic_digital") {
		ui->setLayoutMode->setEnabled(true);
		ui->setDigialSimulationMode->setEnabled(true);
		ui->setAnalogSimulationMode->setEnabled(true);
		ui->setSynthesisMode->setEnabled(true);
		ui->projectSettings->setEnabled(true);
	}
	if(project->getProjectType()=="asic_analog" || project->getProjectType()=="macro") {
		ui->setLayoutMode->setEnabled(true);
		ui->setAnalogSimulationMode->setEnabled(true);
		ui->projectSettings->setEnabled(true);
	}

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

	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
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
