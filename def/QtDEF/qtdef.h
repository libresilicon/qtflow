#ifndef QTDEF_H
#define QTDEF_H

#include <QMainWindow>

class QtDEF : public QMainWindow
{
	Q_OBJECT

public:
	QtDEF(QWidget *parent = 0);
	~QtDEF();
};

#endif // QTDEF_H
