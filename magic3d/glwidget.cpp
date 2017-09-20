#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):
	m_mainWindow(parent)
{
}

void GLWidget::resizeGL(int, int)
{
}

void GLWidget::paintGL()
{
	QColor color("white");
	glClearColor(color.redF(), color.greenF(), color.blueF(), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// draw stuff here...

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	update();
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
}
