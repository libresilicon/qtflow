#ifndef WAVE_H
#define WAVE_H

#include "vcdtreemodel.h"
#include "vcdlistmodel.h"

#include <QGraphicsScene>
#include <QWidget>

namespace Ui {
class Wave;
}

class Wave : public QWidget
{
    Q_OBJECT

public:
    explicit Wave(QWidget *parent = 0);
    ~Wave();

    void loadVcd(QString);

public slots:
    void onSignalsChanged();

private:
    Ui::Wave *ui;
    VcdTreeModel *tree;
    VcdListModel *list;
    QGraphicsScene *scene;

    void drawSignals();
};

#endif // WAVE_H
