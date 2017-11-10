#ifndef CONTACTSVIEW_H
#define CONTACTSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include <complex>

class ContactsView : public QGraphicsView
{
public:
	ContactsView(QWidget *parent = Q_NULLPTR);
public slots:
	 void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
};

#endif // CONTACTSVIEW_H
