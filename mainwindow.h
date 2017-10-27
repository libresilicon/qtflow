#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QString>
#include <QProcess>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QErrorMessage>

#include <iostream>
#include <string>

#include "ui_mainwindow.h"

#include "projectselector.h"
#include "fileselector.h"
#include "moduleselector.h"

#include "buildflow.h"

#include "editortabmanager.h"

#include "projectsettings.h"
#include "pyprojectsettings.h"

#include "templates.h"
#include "welcome.h"
#include "settings.h"
#include "project.h"
#include "new.h"
#include "iopads.h"
#include "wave.h"
#include "pysettings.h"

#include "pythonconsoledockwidget.h"

#include "qlayout/drcsettings.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QCommandLineParser *parser = nullptr, PythonQtObjectPtr context = nullptr);
	~MainWindow();

	void enableProject();
	void disableProject();

	void enableFile();
	void disableFile();

	void error(QString);

private:
	void openProject(QString file);
	bool isCode(QString suffix);
	bool isSchematic(QString suffix);
	void hideAllDockerWidgets();
	void disableAllFunctions();
	void enableAllFunctions();

private slots:
	void on_MainWindow_destroyed();

	void on_newProject_triggered();
	void on_openProject_triggered();

	void on_saveFile_triggered();

	void on_exit_triggered();

	void on_buildAll_triggered();
	void on_menuModules_triggered();
	void on_menuSimulation_triggered();
	void on_menuSynthesis_triggered();
	void on_menuPlacement_triggered();
	void on_menuRouting_triggered();
	void on_actionPythonShell_triggered();
	void on_actionWaveViewer_triggered();

	void on_menuIOPads_triggered();
	void on_menuSettings_triggered();

	void on_toolRefresh_triggered();

	void on_newFile_triggered();

	void openRecentProject();

	void on_setDigitalSimulationMode_triggered();
	void on_setAnalogSimulationMode_triggered();
	void on_setSynthesisMode_triggered();
	void on_setLayoutMode_triggered();
	void on_actionDRC_triggered();

	void on_synthesisSettings_triggered(bool);
	void on_librarySettings_triggered(bool);

	void onProjectCreated(QString);
	void setTopLevel(QString);
	void setTestBench(QString);
	void syncSettings();
	void onCurrentChanged(int);

private:
	Ui::MainWindow *ui;

	Project *project;
	QErrorMessage *errorMessage;
	New *createWidget;
	Welcome *welcomeWidget;
	Wave *timingWidget;
	IOPads *iopadsWidget;

	QSettings *settings;

	Settings *settingsDialog;
	ProjectSettings *projectSettingsDialog;
	BuildFlow *buildFlowConfig;

	QCommandLineParser *parser;

	FileSelector *filesWidget;
	ProjectSelector *projectsWidget;
	ModuleSelector *modulesWidget;
	PythonConsoleDockWidget *pythonConsoleWidget;
	Templates *newProjectDialog;
	DRCSettings *dialogDRCSettings;

	EditorTabManager *editArea;

	PythonQtObjectPtr mainContext;
};

#endif // MAINWINDOW_H
