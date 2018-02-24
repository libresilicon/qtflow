#ifndef SYMBOLEDITOR_H
#define SYMBOLEDITOR_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsView>
#include <QLabel>

#include "symboleditorscene.h"

class SymbolEditor : public QGraphicsView
{
	Q_OBJECT

public:
	SymbolEditor(QWidget *parent = 0);

	SymbolEditorScene *editScene;
};

#endif // SYMBOLEDITOR_H
