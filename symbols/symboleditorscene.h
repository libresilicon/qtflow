#ifndef SYMBOLEDITORSCENE_H
#define SYMBOLEDITORSCENE_H

#include <QGraphicsScene>

class SymbolEditorScene : public QGraphicsScene
{
public:
	SymbolEditorScene(QObject *parent = Q_NULLPTR);
	SymbolEditorScene(const QRectF &sceneRect, QObject *parent = Q_NULLPTR);
	SymbolEditorScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = Q_NULLPTR);

};

#endif // SYMBOLEDITORSCENE_H
