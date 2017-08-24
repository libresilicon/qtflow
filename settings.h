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
	

private slots:
	void on_selectYosys_triggered();
	void on_selectGraywolf_triggered();
	void on_selectQRouter_triggered();
	void on_buttonBox_save();
	void on_buttonBox_close();

private:
	Ui::Settings *ui;
	QSettings *settings;

};

#endif // Settings_H
