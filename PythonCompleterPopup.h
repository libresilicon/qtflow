#ifndef PYTHONCOMPLETERPOPUP_H
#define PYTHONCOMPLETERPOPUP_H

#include <QListView>

class PythonCompleterPopup : public QListView
{
    Q_OBJECT
public:
    explicit PythonCompleterPopup(QWidget *parent = 0);
    
signals:
    void aboutToShow();                                     //!< emitted when the listview is shown
    void aboutToHide();                                     //!< emitted when the listview is hidden

protected:
    void showEvent(QShowEvent *e);                          //!< overwritten from base class to emit aboutToShow
    void hideEvent(QHideEvent *e);                          //!< overwritten from base class to emit aboutToHide
};

#endif /* PYTHONCOMPLETERPOPUP_H */
