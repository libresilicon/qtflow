#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "project.h"
#include "common.h"
#include "dependencies.h"
#include "new.h"
#include "iopads.h"
#include "wave.h"
#include "welcome.h"
#include "settings.h"

#include "moduleselector.h"
#include "fileselector.h"
#include "projectselector.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QErrorMessage>
#include <QMdiArea>

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

public slots:
	void fireTcsh();
	void errorTcsh();
	void exitTcsh(int);
	void onTopModuleChanged();

private slots:
	void on_MainWindow_destroyed();

	void on_newProject_triggered();
	void on_openProject_triggered();

	void on_openMagicFile_triggered();

	void on_saveFile_triggered();

	void on_exit_triggered();

	void on_buildAll_triggered();
	void on_menuModules_triggered();
	void on_menuSynthesis_triggered();
	void on_menuPlacement_triggered();
	void on_menuRouting_triggered();

	void on_menuIOPads_triggered();
	void on_menuSettings_triggered();

	void on_mainWelcome_clicked();
	void on_mainEdit_clicked();

	void on_tcshExpand_clicked();

	void on_tcshErrors_clicked();

	void on_toolRefresh_triggered();

	void on_newFile_triggered();

	void on_buildVcd_triggered();

	void on_mainTiming_clicked();

	void openRecentProject();

	void openFile(QString);

	void setTopLevel(QString);

	void hideAllDockerWidgets();

private:
	Ui::MainWindow *ui;

	Project *project;
	Dependencies *dependencies;
	QErrorMessage *errorMessage;
	QProcess *tcsh;
	New *createWidget;
	Welcome *welcomeWidget;
	Wave *timingWidget;
	IOPads *iopads;

	QSettings *settings;
	Settings *settingsDialog;
	QCommandLineParser *parser;

	FileSelector *filesWidget;
	ProjectSelector *projectsWidget;
	ModuleSelector *modulesWidget;

	QTabWidget *editArea;
};

#endif // MAINWINDOW_H
