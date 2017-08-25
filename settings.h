#ifndef Settings_H
#define Settings_H

#include "ui_settings.h"

#include <QDialog>
#include <QSettings>

namespace Ui {
	class Settings;
}

class Settings : public QDialog
{
	Q_OBJECT

public:
	Settings(QWidget *parent, QSettings *s);

signals:
	void syncSettings();

private slots:
	void selectYosys_triggered();
	void selectGraywolf_triggered();
	void selectQRouter_triggered();
	void buttonBox_save();
	void buttonBox_close();

private:
	Ui::Settings *ui;
	QSettings *settings;

};

#endif // Settings_H
