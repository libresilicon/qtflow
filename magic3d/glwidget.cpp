#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent):
	m_mainWindow(parent),
	m_fAngle1(0),
	m_fAngle2(0),
	m_fAngle3(0),
	m_fScale(1.0),
	m_wireScale(0),
	m_offsetX(0),
	m_offsetY(0),
	magicdata(NULL),
	lefdata(NULL),
	project(NULL)
{
}

void GLWidget::loadFile(QString file)
{
	layer_rects_t layers;
	rects_t layer;
	QString filedest;
	QTemporaryDir temporaryDir;
	filePath = file;
	if(magicdata) delete magicdata;
	magicdata = new magic::MagicData(file);

	if(project->getTechnology()==magicdata->getTechnology()) {
		if(lefdata) delete lefdata;
		lefdata = new lef::LEFData();
		foreach(QString filename, project->getProcessFiles()) {
			filedest = temporaryDir.path()+"/cells.lef";
			QFile::copy(filename, filedest);
			if(QFile(filedest).exists()) {
				lefdata->loadFile(filedest);
			}
		}
	}

	layers = magicdata->getRectangles();
	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (rect_t e, layer)
		{
			if(e.x1 > m_wireScale) {
				m_wireScale = e.x1;
			}
			if(e.x2 > m_wireScale) {
				m_wireScale = e.x2;
			}
			if(e.y1 > m_wireScale) {
				m_wireScale = e.y1;
			}
			if(e.y2 > m_wireScale) {
				m_wireScale = e.y2;
			}
		}
	}
}

void GLWidget::paintGL()
{
	rects_t layer;
	layer_rects_t layers = magicdata->getRectangles();

	foreach(QString layerN, layers.keys()) {
		layer = layers[layerN];
		foreach (rect_t e, layer) {
			addWire(layerN, e.x1, e.y1, e.x2, e.y2);
		}
	}
}

void GLWidget::initializeGL()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	qglClearColor( Qt::white );
}

void GLWidget::addWire(QString layerN, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	x1/=m_wireScale;
	y1/=m_wireScale;
	x2/=m_wireScale;
	y2/=m_wireScale;

	x1*=m_fScale;
	y1*=m_fScale;
	x2*=m_fScale;
	y2*=m_fScale;

	x1+=m_offsetX;
	x2+=m_offsetX;
	y1+=m_offsetY;
	y2+=m_offsetY;

	QColor color = project->colorMat(layerN);

	qDebug() << __FUNCTION__ << '\t' << x1 << '\t' << y1 << '\t' << x2 << '\t' << y2;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(color.redF(), color.greenF(), color.blueF());

	glBegin(GL_POLYGON);

	glColor3f( 1.0, 0.0, 0.0 );

	glVertex3f( x1, y1, 0 );
	glVertex3f( x1, y2, 0 );
	glVertex3f( x2, y1, 0 );
	glVertex3f( x2, y2, 0 );

	glEnd();

	update();
}

void GLWidget::setProject(Project *p)
{
	project = p;
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		m_fAngle1+=dx;
		m_fAngle2+=dy;
	}

	glRotatef( m_fAngle1, 0.0f, 1.0f, 0.0f );
	//glRotatef( m_fAngle2, 0.0f, 0.0f, 1.0f );

	update();

   lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	int numSteps = event->delta()/120;
	if( event->modifiers() & Qt::ShiftModifier ) {
		m_offsetX+=numSteps;
	} else if( event->modifiers() & Qt::ControlModifier ) {
		m_offsetY+=numSteps;
	} else {
		if((m_fScale-numSteps)>0) {
			m_fScale-=numSteps;
			event->accept();
		}
	}
}
