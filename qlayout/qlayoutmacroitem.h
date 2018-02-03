#ifndef QLAYOUTMACROITEM_H
#define QLAYOUTMACROITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QMenu>
#include <QPen>
#include <QtMath>

#include "qlayoutwireitem.h"
#include "qlayoutmacropinitem.h"

class QLayoutMacroItem : public QGraphicsRectItem
{
public:
	explicit QLayoutMacroItem(qreal scale, QLayoutMacroItem *orig);
	explicit QLayoutMacroItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = Q_NULLPTR);

	void addRectangle(QString layer, QBrush brush, QRectF rect);
	void addPolygon(QString layer, QBrush brush, QPolygonF poly);
	void clearPolygons();
	void clearRectangles();

	QLayoutMacroPinItem* addPin(QString name);

	void setMacroName(QString);
	QString getMacroName();

	void setInstanceName(QString);
	QString getInstanceName();

	void setSize(qreal w, qreal h);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:

public slots:

private:
	bool m_dragged;
	QString m_macroName;
	QString m_instanceName;
	qreal m_width;
	qreal m_height;

	QGraphicsSimpleTextItem *m_macroNameLabel;
	QGraphicsSimpleTextItem *m_instanceNameLabel;

	QMap<QString,QVector<QGraphicsRectItem*>> m_layerList;
	QMap<QString,QVector<QGraphicsRectItem*>> m_layerGDSList;
	QVector<QLayoutMacroPinItem*> m_pinList;
};

#endif // QLAYOUTMACROITEM_H
