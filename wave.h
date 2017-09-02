#ifndef WAVE_H
#define WAVE_H

#include "vcd/vcdtreemodel.h"
#include "vcd/vcdlistmodel.h"

#include <QGraphicsScene>
#include <QDockWidget>
#include <QDebug>
#include <QFile>
#include <QTreeView>
#include <QListView>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QMimeData>

namespace Ui {
class Wave;
}

class Wave : public QDockWidget
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
