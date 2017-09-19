#ifndef MAGIC3D_H
#define MAGIC3D_H

#include <QDialog>

#include "ui_magic3d.h"
#include "project.h"

class Magic3D : public QDialog
{
	Q_OBJECT

	public:
		Magic3D(QWidget *parent = nullptr);
		void setProject(Project *p);
		void loadFile(QString file);

	private:
		Ui::Magic3D *ui;
		Project *project;
};

#endif // MAGIC3D_H