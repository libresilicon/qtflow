#ifndef WAVE_H
#define WAVE_H

#include "vcdtreemodel.h"

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

private:
    Ui::Wave *ui;
    VcdTreeModel *tree;
};

#endif // WAVE_H
