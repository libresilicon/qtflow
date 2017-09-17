#ifndef LAYOUTVISIBLES_H
#define LAYOUTVISIBLES_H

#include <QDockWidget>

#include "ui_layoutvisibles.h"

class LayoutVisibles : public QDockWidget
{
	Q_OBJECT
public:
	explicit LayoutVisibles(QWidget *parent = nullptr);

signals:

public slots:

private:
	Ui::LayoutVisibles *ui;
};

#endif // LAYOUTVISIBLES_H
