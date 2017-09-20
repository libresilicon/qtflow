#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):
	m_mainWindow(parent),
	m_fAngle1(0),
	m_fAngle2(0),
	m_fAngle3(0),
	m_fScale(1.0),
	m_context(NULL)
{
}

void GLWidget::resizeGL(int, int)
{
}

void GLWidget::paintGL()
{
	QMutexLocker locker(&m_windowLock);

	QMatrix4x4 modelview;
	QColor back("white");
	QColor front("red");

	modelview.rotate(m_fAngle1, 0.0f, 1.0f, 0.0f);
	modelview.rotate(m_fAngle2, 1.0f, 0.0f, 0.0f);
	modelview.rotate(m_fAngle3, 0.0f, 0.0f, 1.0f);
	modelview.scale(m_fScale);
	modelview.translate(0.0f, -0.2f, 0.0f);

	glClearColor(back.redF(), back.greenF(), back.blueF(), 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_program->bind();
	m_vbo.bind();

	m_program->enableAttributeArray(vertexAttr);
	m_program->enableAttributeArray(normalAttr);
	m_program->setAttributeBuffer(vertexAttr, GL_FLOAT, 0, 3);
	const int verticesSize = vertices.count() * 3 * sizeof(GLfloat);
	m_program->setAttributeBuffer(normalAttr, GL_FLOAT, verticesSize, 3);

	m_program->setUniformValue(matrixUniform, modelview);
	m_program->setUniformValue(colorUniform, front);

	m_context->functions()->glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	update();
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	if(m_context) delete m_context;
	m_context = new QOpenGLContext(this);

	m_vbo.create();

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	vshader->compileSourceCode(
		"attribute highp vec4 vertex;"
		"attribute mediump vec3 normal;"
		"uniform mediump mat4 matrix;"
		"uniform lowp vec4 sourceColor;"
		"varying mediump vec4 color;"
		"void main(void)"
		"{"
		"    vec3 toLight = normalize(vec3(10.0, 10.0, 10.0));"
		"    float angle = max(dot(normal, toLight), 0.0);"
		"    vec3 col = sourceColor.rgb;"
		"    color = vec4(col*0.5 + col*0.2*angle, 0.5);"
//		"    color = vec4(col * 0.2 + col * 0.6 * angle, 1.0);"
//		"    color = clamp(color, 0.0, 0.5);"
		"    gl_Position = matrix * vertex;"
		"}");

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	fshader->compileSourceCode(
		"varying mediump vec4 color;"
		"void main(void)"
		"{"
		"    gl_FragColor = color;"
		"}");

	m_program = new QOpenGLShaderProgram;
	m_program->addShader(vshader);
	m_program->addShader(fshader);
	m_program->link();
	m_program->bind();

	vertexAttr = m_program->attributeLocation("vertex");
	normalAttr = m_program->attributeLocation("normal");
	matrixUniform = m_program->uniformLocation("matrix");
	colorUniform = m_program->uniformLocation("sourceColor");

	createGeometry();

	m_vbo.bind();
	const int verticesSize = vertices.count() * 3 * sizeof(GLfloat);
	m_vbo.allocate(verticesSize * 2);
	m_vbo.write(0, vertices.constData(), verticesSize);
	m_vbo.write(verticesSize, normals.constData(), verticesSize);
}

void GLWidget::createGeometry()
{
	vertices.clear();
	normals.clear();

	qreal x1 = +0.06f;
	qreal y1 = -0.14f;

	qreal x2 = +0.14f;
	qreal y2 = -0.06f;

	quad(x1, y1, x2, y2, y2, x2, y1, x1);

	extrude(x1, y1, x2, y2);
	extrude(x2, y2, y2, x2);
	extrude(y2, x2, y1, x1);
	extrude(y1, x1, x1, y1);
}

void GLWidget::quad(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal x4, qreal y4)
{
	vertices << QVector3D(x1, y1, -0.05f);
	vertices << QVector3D(x2, y2, -0.05f);
	vertices << QVector3D(x4, y4, -0.05f);

	vertices << QVector3D(x3, y3, -0.05f);
	vertices << QVector3D(x4, y4, -0.05f);
	vertices << QVector3D(x2, y2, -0.05f);

	QVector3D n = QVector3D::normal(QVector3D(x2-x1, y2-y1, 0.0f), QVector3D(x4-x1, y4-y1, 0.0f));

	normals << n;
	normals << n;
	normals << n;

	normals << n;
	normals << n;
	normals << n;

	vertices << QVector3D(x4, y4, 0.05f);
	vertices << QVector3D(x2, y2, 0.05f);
	vertices << QVector3D(x1, y1, 0.05f);

	vertices << QVector3D(x2, y2, 0.05f);
	vertices << QVector3D(x4, y4, 0.05f);
	vertices << QVector3D(x3, y3, 0.05f);

	n = QVector3D::normal(QVector3D(x2-x4, y2-y4, 0.0f), QVector3D(x1-x4, y1-y4, 0.0f));

	normals << n;
	normals << n;
	normals << n;

	normals << n;
	normals << n;
	normals << n;
}

void GLWidget::extrude(qreal x1, qreal y1, qreal x2, qreal y2)
{
	vertices << QVector3D(x1, y1, +0.05f);
	vertices << QVector3D(x2, y2, +0.05f);
	vertices << QVector3D(x1, y1, -0.05f);

	vertices << QVector3D(x2, y2, -0.05f);
	vertices << QVector3D(x1, y1, -0.05f);
	vertices << QVector3D(x2, y2, +0.05f);

	QVector3D n = QVector3D::normal(QVector3D(x2-x1, y2-y1, 0.0f), QVector3D(0.0f, 0.0f, -0.1f));

	normals << n;
	normals << n;
	normals << n;

	normals << n;
	normals << n;
	normals << n;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		m_fAngle1+=dx;
		m_fAngle2+=dy;
	}

   lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if((m_fScale+(numSteps))>0) {
		m_fScale+=(numSteps);
		event->accept();
	}
}
