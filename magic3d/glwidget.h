#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWidget>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent);

protected:
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void initializeGL() override;

private:

	QWidget *m_mainWindow;
};

#endif // GLWIDGET_H
