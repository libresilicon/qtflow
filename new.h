#ifndef NEW_H
#define NEW_H

#include <QDialog>
#include <QFileInfo>

#include "project.h"

enum new_element_t
{
	Verilog,
	VerilogTestbench,
	Schematics,
	Symbol,
	Magic
};

namespace Ui {
	class New;
}

class New : public QDialog
{
	Q_OBJECT

public:
	explicit New(QWidget *parent = 0);
	~New();

	void setProject(Project *p);

signals:
	void fileCreated(QString);

private slots:
	void on_buttonBox_accepted();

private:
	QFileInfo createMagicFile();
	QString fileExtension(new_element_t e);

	Ui::New *ui;
	Project *project;
};

#endif // NEW_H
