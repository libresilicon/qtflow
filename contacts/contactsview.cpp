#include "contactsview.h"

ContactsView::ContactsView(QWidget *parent) :
	QGraphicsView(parent)
{

}

void ContactsView::wheelEvent(QWheelEvent *event)
{
	qreal factor = pow(1.01, event->delta());
	scale(factor, factor);
}
