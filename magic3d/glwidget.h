#ifndef GLWIDGET_H
#define GLWIDGET_H

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
#include "project.h"

typedef QVector<QVector3D> shapes;

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent);
	void loadFile(QString file);
	void setProject(Project *p);

protected:
	void paintGL() override;
	void initializeGL() override;
	void resizeGL(int w, int h);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	void addWire(QString layerN, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

	QWidget *m_mainWindow;

	qreal m_fAngle1, m_fAngle2, m_fAngle3;
	qreal m_fScale;

	qreal m_lowestX, m_highestX;
	qreal m_lowestY, m_highestY;

	qreal m_offsetX, m_offsetY;

	QPoint lastPos;
	QString filePath;
	magic::MagicData *magicdata;
	lef::LEFData *lefdata;
	Project *project;
};

#endif // GLWIDGET_H
