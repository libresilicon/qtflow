#include "PythonCompleterPopup.h"

#include <QShowEvent>
#include <QHideEvent>


PythonCompleterPopup::PythonCompleterPopup(QWidget *parent) :
    QListView(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}


void PythonCompleterPopup::showEvent(QShowEvent *e)
{
    if( !e->spontaneous() )
        emit aboutToShow();
}

void PythonCompleterPopup::hideEvent(QHideEvent *e)
{
    if( !e->spontaneous() )
        emit aboutToHide();
}
