#include "mainwindow.h"

MainWindow::MainWindow(QCommandLineParser *p, PythonQtObjectPtr context ) :
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

	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qtflow");
	settingsDialog = new Settings(this, settings);
	connect(settingsDialog, SIGNAL(syncSettings()), this, SLOT(syncSettings()));
	mainContext.addObject("settings", new PySettings(this, settings));

	projectSettingsDialog = new ProjectSettings(this);
	connect(ui->projectSettings, SIGNAL(triggered(bool)), projectSettingsDialog, SLOT(open()));

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
	connect(modulesWidget, SIGNAL(setTestBench(QString)), filesWidget, SLOT(refresh()));
	connect(modulesWidget, SIGNAL(setTopLevel(QString)), filesWidget, SLOT(refresh()));

	timingWidget = new Wave(this);
	timingWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::BottomDockWidgetArea, timingWidget);

	buildFlowConfig = new BuildFlow(this);
	connect(ui->buildFlowSettings, SIGNAL(triggered(bool)), buildFlowConfig, SLOT(open()));

	pythonConsoleWidget = new PythonConsoleDockWidget(this, mainContext);
	pythonConsoleWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea );
	addDockWidget(Qt::BottomDockWidgetArea, pythonConsoleWidget);

	editArea = new EditorTabManager(ui->centralWidget);
	connect(filesWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(projectsWidget, SIGNAL(openFile(QString)), editArea, SLOT(openFile(QString)));
	connect(editArea,SIGNAL(fileSaved()),modulesWidget,SLOT(refresh()));
	connect(editArea,SIGNAL(currentChanged(int)),this,SLOT(onCurrentChanged(int)));

	newProjectDialog = new Templates(this, settings, mainContext);
	connect(newProjectDialog,SIGNAL(projectCreated(QString)),this,SLOT(onProjectCreated(QString)));

	connect(createWidget, SIGNAL(fileCreated(QString)), editArea, SLOT(openFile(QString)));

	QMenu *recent = ui->menuRecentProjects;
	QAction *recent_action;

	settings->beginGroup("history");
	QStringList reverseProjectsList = settings->value("recentProjects").toStringList();
	settings->endGroup();

	projectSettingsDialog->setSettings(settings);

	specialNetDialog = new SpecialNets(this);

	QStringList recentProjectsList;
	QStringList::const_reverse_iterator rev;
	for(rev = reverseProjectsList.rbegin(); rev != reverseProjectsList.rend(); ++rev)
		recentProjectsList << *rev;

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

void MainWindow::onCurrentChanged(int id)
{
	EditorWidget *w = (EditorWidget *)editArea->widget(id);
	if(w) {
		switch(w->getType()) {
			case DEFLayoutEditorWidgetType:
				on_setLayoutMode_triggered();
				break;
			case MagicLayoutEditorWidgetType:
				on_setLayoutMode_triggered();
				break;
			case VerilogCodeEditorWidgetType:
				on_setDigitalSimulationMode_triggered();
				break;
			default:
				break;
		}
	}
}

void MainWindow::disableAllFunctions()
{
	ui->setLayoutMode->setEnabled(false);
	ui->setDigitalSimulationMode->setEnabled(false);
	ui->setAnalogSimulationMode->setEnabled(false);
	ui->setSynthesisMode->setEnabled(false);
	ui->projectSettings->setEnabled(false);
	ui->buildFlowSettings->setEnabled(false);
	ui->synthesisSettings->setEnabled(false);
	ui->librarySettings->setEnabled(false);
	ui->actionDRC->setEnabled(false);
	ui->actionWaveViewer->setEnabled(false);
}

void MainWindow::enableAllFunctions()
{
	ui->setLayoutMode->setEnabled(true);
	ui->setDigitalSimulationMode->setEnabled(true);
	ui->setAnalogSimulationMode->setEnabled(true);
	ui->setSynthesisMode->setEnabled(true);
	ui->projectSettings->setEnabled(true);
	ui->buildFlowSettings->setEnabled(true);
	ui->synthesisSettings->setEnabled(true);
	ui->librarySettings->setEnabled(true);
	ui->actionDRC->setEnabled(true);
	ui->actionWaveViewer->setEnabled(true);
}

void MainWindow::on_synthesisSettings_triggered(bool)
{
}

void MainWindow::on_librarySettings_triggered(bool)
{

}

void MainWindow::hideAllDockerWidgets()
{
	filesWidget->setVisible(false);
	projectsWidget->setVisible(false);
	modulesWidget->setVisible(false);
	timingWidget->setVisible(false);
	pythonConsoleWidget->setVisible(false);
}

void MainWindow::openProject(QString path)
{
	QFile project_path(path);
	if (project_path.exists()) {
		if(project) delete project;
		project = new Project(settings, path, mainContext);
		connect(project,SIGNAL(simulationDone()),this,SLOT(on_menuSimulation_triggered()));
		modulesWidget->setProject(project);
		filesWidget->setProject(project);
		projectsWidget->setProject(project);
		projectSettingsDialog->setProject(project);
		editArea->setProject(project);
		buildFlowConfig->setProject(project);
		createWidget->setProject(project);
		specialNetDialog->setProject(project);
		contactPlacementDialog->setProject(project);
		mainContext.addObject("project_settings", new PyProjectSettings(project));
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
	delete createWidget;
	delete welcomeWidget;
	delete timingWidget;
}

void MainWindow::on_MainWindow_destroyed()
{
}

void MainWindow::on_setDigitalSimulationMode_triggered()
{
	hideAllDockerWidgets();
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
}

void MainWindow::on_setAnalogSimulationMode_triggered()
{
	hideAllDockerWidgets();
	//toolBoxWidgetTestBench->setVisible(true);
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
}

void MainWindow::on_setSynthesisMode_triggered()
{
	hideAllDockerWidgets();
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
	pythonConsoleWidget->setVisible(true);
}

void MainWindow::on_setLayoutMode_triggered()
{
	hideAllDockerWidgets();
	filesWidget->setVisible(true);
	projectsWidget->setVisible(true);
	modulesWidget->setVisible(true);
}

void MainWindow::on_newProject_triggered()
{
	newProjectDialog->show();
}

void MainWindow::on_actionPythonShell_triggered()
{
	pythonConsoleWidget->setVisible(!pythonConsoleWidget->isVisible());
}

void MainWindow::on_actionWaveViewer_triggered()
{
	qDebug() << "Loading " << project->getVCDPath();
	timingWidget->setVisible(!timingWidget->isVisible());
	timingWidget->loadVcd(project->getVCDPath());
}

void MainWindow::on_actionSpecialNets_triggered()
{
	specialNetDialog->show();
}

void MainWindow::onProjectCreated(QString s)
{
	openProject(s);
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

void MainWindow::on_toolRefresh_triggered()
{
	enableProject();
}

void MainWindow::enableProject()
{
	if(!project) return;
	//QStringList filter;

	enableAllFunctions();

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

	filesWidget->refresh();
	projectsWidget->refresh();
	modulesWidget->refresh();
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
