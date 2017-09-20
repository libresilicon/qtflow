#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QWidget>

#include <qmath.h>

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
	void createGeometry();
	void quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4);
	void extrude(qreal x1, qreal y1, qreal x2, qreal y2);

	QWidget *m_mainWindow;
	QOpenGLShaderProgram *m_program;
	QOpenGLBuffer m_vbo;
	QOpenGLContext *m_context;
	qreal m_fAngle;
	qreal m_fScale;

	int vertexAttr;
	int normalAttr;
	int matrixUniform;
	int colorUniform;

	QVector<QVector3D> vertices;
	QVector<QVector3D> normals;
};

#endif // GLWIDGET_H
