#ifndef GRID_H
#define GRID_H

#include "common.h"
#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class Grid;
}

class Grid : public QWidget
{
    Q_OBJECT

public:
    explicit Grid(QWidget *parent = 0);
    ~Grid();

    void RenderRectangles(rects_t rectangles);

private:
    Ui::Grid *ui;
    QGraphicsScene scene;

    QColor colorMat(QString m);
};

#endif // GRID_H
