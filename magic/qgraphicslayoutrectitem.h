#ifndef QGRAPHICSLAYOUTRECTITEM_H
#define QGRAPHICSLAYOUTRECTITEM_H

#include <QObject>
#include <QWidget>

class QGraphicsLayoutRectItem : public QWidget
{
	Q_OBJECT
public:
	explicit QGraphicsLayoutRectItem(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // QGRAPHICSLAYOUTRECTITEM_H