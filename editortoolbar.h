#ifndef EDITORTOOLBAR_H
#define EDITORTOOLBAR_H

#include "ui_editortoolbar.h"

#include <QDockWidget>

class EditorToolBar : public QDockWidget
{
	Q_OBJECT
public:
	explicit EditorToolBar(QWidget *parent = nullptr);

signals:

public slots:
private:
	Ui::EditorToolBar *ui;
};

#endif // EDITORTOOLBAR_H
