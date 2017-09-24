#ifndef MAGIC3D_H
#define MAGIC3D_H

#include <QDialog>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVBoxLayout>
#include <QToolBar>

#include "project.h"
#include "glwidget.h"

class Magic3D : public QDialog
{
	Q_OBJECT

	public:
		Magic3D(QWidget *parent = nullptr);
		void setProject(Project *p);
		void loadFile(QString file);

	private:
		Project *project;
		GLWidget *view;
		QVBoxLayout *layout;
		QToolBar *toolbar;
};

#endif // MAGIC3D_H
