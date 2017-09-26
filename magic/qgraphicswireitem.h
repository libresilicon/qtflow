#ifndef QGRAPHICSWIREITEM_H
#define QGRAPHICSWIREITEM_H

#include <QObject>
#include <QWidget>

class QGraphicsWireItem : public QWidget
{
	Q_OBJECT
public:
	explicit QGraphicsWireItem(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // QGRAPHICSWIREITEM_H