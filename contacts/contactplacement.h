#ifndef CONTACTPLACEMENT_H
#define CONTACTPLACEMENT_H

#include <QDialog>
#include <QDir>
#include <QComboBox>
#include <QGraphicsScene>
#include <QGroupBox>
#include <QToolBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QLabel>

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
	void sideLength_changed(QString);
	void m_padNames_cellChanged(int i, int j);
	void padNames_select_changed(int i);
	void open();

private:
	void addTables();
	void refreshTables();
	void storeTables();

	void refreshNameTable();
	void storeNameTable();

	Project* project;
	Ui::ContactPlacement* ui;
	QGraphicsScene* scene;
	ContactsView* view;
	QRectF m_baseRect;
	bool m_tableComplete;
	PadInfo* m_padInfo;
	QMap<QString,QTableWidget*> m_tables;
	QMap<QString,QMap<QString,int>> m_tablesPinMapping;
	QTableWidget* m_padNames;
	QLineEdit* m_sideLength;
	blif::BLIFData *m_blifdata;
};

#endif // CONTACTPLACEMENT_H
