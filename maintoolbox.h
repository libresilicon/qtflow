#ifndef MainToolBox_H
#define MainToolBox_H

#include <QDockWidget>
#include <QIcon>
#include <QRect>
#include <QPixmap>
#include <QToolButton>
#include <QPainter>

class MainToolBox : public QDockWidget
{
	Q_OBJECT
public:
	explicit MainToolBox(QWidget *parent = nullptr);

private:
	QIcon createColorToolButtonIcon(const QString &imageFile, QColor color);

signals:

public slots:
private:
	QToolButton *saveButton;
};

#endif // MainToolBox_H
