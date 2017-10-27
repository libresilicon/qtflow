#include "symboleditor.h"

SymbolEditor::SymbolEditor(QWidget *parent) :
	QGraphicsView(parent),
	editScene(new SymbolEditorScene(this))
{
	SymbolEditorScene *editScene;
	setScene(editScene);
}
