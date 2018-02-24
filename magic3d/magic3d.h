#ifndef MAGIC3D_H
#define MAGIC3D_H

#include <QDialog>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVBoxLayout>
#include <QToolBar>

#include "glwidget.h"

class Magic3D : public QDialog
{
	Q_OBJECT

	public:
		Magic3D(QWidget *parent = nullptr);
		void loadFile(QString file);

	private:
		GLWidget *view;
		QVBoxLayout *layout;
		QToolBar *toolbar;
};

#endif // MAGIC3D_H
