#include "editortoolbar.h"

EditorToolBar::EditorToolBar(QWidget *parent) :
	QDockWidget(parent)
{
	saveButton = new QToolButton(this);
	saveButton->setIcon(createColorToolButtonIcon("images/document-save.svg", Qt::white));
}

QIcon EditorToolBar::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
	QPixmap pixmap(50, 50);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	QPixmap image(imageFile);
	// Draw icon centred horizontally on button.
	QRect target(4, 0, 42, 43);
	QRect source(0, 0, 42, 43);
	painter.fillRect(QRect(0, 60, 50, 80), color);
	painter.drawPixmap(target, image, source);

	return QIcon(pixmap);
}
