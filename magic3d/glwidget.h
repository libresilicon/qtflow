#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QWidget>
#include <QMouseEvent>
#include <QMutexLocker>
#include <QTemporaryDir>
#include <QGLWidget>

#include <qmath.h>

#include "magic/magicdata.h"
#include "lef/lefdata.h"
#include "gds2/gdtdata.h"

enum orient_mode {
	ORIENT_NONE,
	ORIENT_TOP,
	ORIENT_BOTTOM,
	ORIENT_FRONT,
	ORIENT_BACK,
	ORIENT_LEFT,
	ORIENT_RIGHT,
	ORIENT_SIDE
};

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent);
	void loadFile(QString file);

public slots:
	void keyPressEvent(QKeyEvent *event) override;
	void axis3d_back();
	void axis3d_bottom();
	void axis3d_front();
	void axis3d_left();
	void axis3d_right();
	void axis3d_top();
	void axis3d_side();

protected:
	void paintGL() override;
	void initializeGL() override;
	void resizeGL(int w, int h);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void addBox(QString layerN, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	void addWires();
	void addModules();

	QWidget *m_mainWindow;

	qreal m_fScale;

	qreal m_lowestX, m_highestX;
	qreal m_lowestY, m_highestY;

	qreal m_offsetX, m_offsetY;

	GLfloat m_angle1, m_angle2;

	orient_mode lastOrient;
	QPoint lastPos;
	QString filePath;
	magic::MagicData *magicdata;
};

#endif // GLWIDGET_H
