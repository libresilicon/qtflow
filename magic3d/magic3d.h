#ifndef MAGIC3D_H
#define MAGIC3D_H

#include <QDialog>

#include "ui_magic3d.h"

class Magic3D : public QDialog
{
	Q_OBJECT

	public:
		Magic3D(QWidget *parent = nullptr);
	
	private:
		Ui::Magic3D *ui;
};

#endif // MAGIC3D_H