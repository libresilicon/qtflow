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
#include "testbenchtoolbox.h"
#include "synthesistoolbox.h"

#include "editortabmanager.h"

#include "projectsettings.h"

#include "templates.h"
#include "welcome.h"
#include "settings.h"
#include "project.h"
#include "dependencies.h"
#include "new.h"
#include "iopads.h"
#include "wave.h"
#include "maintoolbox.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QCommandLineParser *parser = 0);
	~MainWindow();

	void enableProject();
	void disableProject();

	void enableTopModule();

	void enableFile();
	void disableFile();

	void error(QString);

private:
	void openProject(QString file);
	bool isCode(QString suffix);
	bool isSchematic(QString suffix);
	void hideAllDockerWidgets();

private slots:
	void on_MainWindow_destroyed();

	void on_newProject_triggered();
	void on_openProject_triggered();

	void on_openMagicFile_triggered();

	void on_saveFile_triggered();

	void on_exit_triggered();

	void on_buildAll_triggered();
	void on_menuModules_triggered();
	void on_menuSimulation_triggered();
	void on_menuSynthesis_triggered();
	void on_menuPlacement_triggered();
	void on_menuRouting_triggered();

	void on_menuIOPads_triggered();
	void on_menuSettings_triggered();

	void on_toolRefresh_triggered();

	void on_newFile_triggered();

	void openRecentProject();

	void on_simulationMode_triggered();
	void on_synthesisMode_triggered();

	void setTopLevel(QString);
	void setTestBench(QString);
	void syncSettings();
	void onTopModuleChanged();

private:
	Ui::MainWindow *ui;

	Project *project;
	Dependencies *dependencies;
	QErrorMessage *errorMessage;
	New *createWidget;
	Welcome *welcomeWidget;
	Wave *timingWidget;
	IOPads *iopadsWidget;

	QSettings *settings;
	Settings *settingsDialog;
	ProjectSettings *projectSettingsDialog;
	QCommandLineParser *parser;

	FileSelector *filesWidget;
	ProjectSelector *projectsWidget;
	ModuleSelector *modulesWidget;

	TestBenchToolBox *toolBoxWidgetTestBench;
	SynthesisToolBox *toolBoxWidgetSynthesis;
	MainToolBox *mainToolBox;

	EditorTabManager *editArea;
};

#endif // MAINWINDOW_H
