#ifndef GRID_H
#define GRID_H

#include <QWidget>
#include <QGraphicsScene>

typedef QVector<QRect> rects_t;

namespace Ui {
	class Grid;
}

class Grid : public QWidget
{
	Q_OBJECT

	public:
		explicit Grid(QWidget *parent = 0);
		~Grid();

		void RenderRectangles(QString layer, rects_t rectangles);

	private:
		Ui::Grid *ui;
		QGraphicsScene *scene;
		QColor colorMat(QString m);
};

#endif // GRID_H
