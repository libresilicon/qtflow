#ifndef EDITORTOOLBAR_H
#define EDITORTOOLBAR_H

#include <QDockWidget>
#include <QIcon>
#include <QRect>
#include <QPixmap>
#include <QToolButton>
#include <QPainter>

class EditorToolBar : public QDockWidget
{
	Q_OBJECT
public:
	explicit EditorToolBar(QWidget *parent = nullptr);

private:
	QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);

signals:

public slots:
private:
	QToolButton *saveButton;
};

#endif // EDITORTOOLBAR_H
