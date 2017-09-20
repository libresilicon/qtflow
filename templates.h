#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "project.h"

#include <QDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QListWidgetItem>


namespace Ui {
class Templates;
}

class Templates : public QDialog
{
	Q_OBJECT

public:
	explicit Templates(QWidget *parent = 0, QSettings *settings = 0, PythonQtObjectPtr *main = 0);
	~Templates();

private slots:
	void on_buttonBox_accepted();
	void on_comboTechnology_activated(int);
	void on_pushPath_clicked();
	void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *b);

signals:
	void projectCreated(QString);

private:
	Ui::Templates *ui;
	Project *project;
	QSettings *settings;
	PythonQtObjectPtr *mainContext;
	QDomDocument *settingsFileProcess;
};

#endif // TEMPLATES_H
