#ifndef WAVE_H
#define WAVE_H

#include "vcd/vcdtreemodel.h"
#include "vcd/vcdlistmodel.h"
#include "vcd/vcdsignaltreemodel.h"

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
	void onSelectScope(QModelIndex);
	void onSelectSignal(QModelIndex);

private:
    Ui::Wave *ui;
    QGraphicsScene *scene;
	VcdTreeModel *tree;
	VcdSignalTreeModel *signalTree;
	VcdListModel *list;

	vcd::VcdData vcd_data;

    void drawSignals();
};

#endif // WAVE_H
