#ifndef CONTACTPLACEMENT_H
#define CONTACTPLACEMENT_H

#include <QDialog>
#include <QDir>
#include <QComboBox>
#include <QGraphicsScene>

#include "ui_contactplacement.h"

#include "project.h"

#include "contactsview.h"
#include "padinfo.h"

#include "blif/blifdata.h"

class ContactPlacement : public QDialog
{
	Q_OBJECT

public:
	ContactPlacement(QWidget *parent = Q_NULLPTR);
	void setProject(Project *p);

public slots:
	void updatePreview();
	void on_buttonBox_accepted();
	void open();

private:
	void refreshTables();

	Project* project;
	Ui::ContactPlacement* ui;
	QGraphicsScene* scene;
	ContactsView* view;
	QRectF m_baseRect;
	bool m_tableComplete;
	PadInfo* m_padInfo;
};

#endif // CONTACTPLACEMENT_H
